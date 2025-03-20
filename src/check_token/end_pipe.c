/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:40:23 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/19 11:53:46 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_inpts(char *first_inpt, char *inpt)
{
	int		i;
	char	*res;
	char	*temp;

	if (!first_inpt || inpt)
		return (NULL);
	i = ft_strlen(first_inpt);
	res = NULL;
	temp = NULL;
	if (inpt[i] && inpt[i] == ' ')
		res = ft_strjoin(first_inpt, inpt);
	else
	{
		temp = ft_strjoin(first_inpt, " ");
		if (!temp)
			return (print_e(1, "pipe", 1, ""), NULL);
		res = ft_strjoin(temp, inpt);
		free(temp);
	}
	if (!res)
		return (print_e(1, "pipe", 1, ""), NULL);
	return (res);
}

int	get_inpt_content(int fd[2])
{
	char	*inpt;

	inpt = NULL;
	while (1)
	{
		inpt = readline("pipe > ");
		if (!inpt)
		{
			close(fd[1]);
			close(fd[0]);
		}
		if (*inpt && !ft_isspace(inpt))
		{
			ft_putstr_fd(inpt, fd[1]);
			break ;
		}
		free(inpt);
	}
	close(fd[1]);
	free(inpt);
	return (0);
}

int	pipe_child(pid_t pid, int fd[2])
{
	int			nbr;
	int			status;
	extern int	g_exit_code;

	status = 0;
	if (!pid)
	{
		nbr = 0;
		set_signal_action(1);
		nbr = get_inpt_content(fd);
		close(fd[0]);
		close(fd[1]);
		exit (nbr);
	}
	waitpid(pid, &status, 0);
	if (status != 0)
	{
		close(fd[0]);
		close(fd[1]);
		g_exit_code = WEXITSTATUS(status);
		return (0);
	}
	return (1);
}

char	*get_inpt(char **frst_inpt, t_list **env, t_token **t)
{
	int			fd[2];
	pid_t		pid;
	char		*res;

	if (pipe(fd) == -1)
		return (print_e(1, "pipe", 1, ""), NULL);
	set_signal_action(2);
	pid = fork();
	if (pid == -1)
		return (print_e(1, "pipe", 1, ""), NULL);
	if (!pid)
	{
		free_token(t);
		free(*frst_inpt);
		free_array((*env)->env_c);
		free_list(*env);
	}
	if (!pipe_child(pid, fd))
		return (NULL);
	set_signal_action(0);
	close(fd[1]);
	res = get_next_line(fd[0]);
	close(fd[0]);
	return (res);
}

int	end_pipe(t_token **cur, char **frst_inpt, t_list **env, t_token **t)
{
	char	*inpt;
	char	*replace_inpt;
	t_token	*token;
	t_list	*copy;

	inpt = get_inpt(frst_inpt, env, t);
	if (!inpt)
		return (0);
	token = lexer(inpt, (*env)->env_c);
	copy = make_list((*env)->env_c);
	copy->env_c = get_env(copy);
	if (!token || !check_token(&token, &inpt, &copy))
		return (free_token(cur), free_list(copy), free_token(&token), 0);
	(*cur)->next = token;
	token->prev = (*cur);
	replace_inpt = join_inpts(*frst_inpt, inpt);
	free(inpt);
	if (replace_inpt)
	{
		free(*frst_inpt);
		(*frst_inpt) = replace_inpt;
	}
	return (free_list(copy), 1);
}
