/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:40:23 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/24 17:45:28 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	exit_code;

char	*join_inpts(char *first_inpt, char *inpt)
{
	int		i;
	char	*res;
	char	*temp;

	i = ft_strlen(first_inpt);
	res = NULL;
	temp = NULL;
	if (inpt[i] == ' ')
		res = ft_strjoin(first_inpt, inpt);
	else
	{
		temp = ft_strjoin(first_inpt, " ");
		if (!temp)
			return (print_error(1, "pipe", "Cannot allocate memory"), NULL);
		res = ft_strjoin(temp, inpt);
		free(temp);
	}
	if (!res)
		return (print_error(1, "pipe", "Cannot allocate memory"), NULL);
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
			return (print_error(1, "malloc", "Cannot allocate memory"), -1);
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

char	*get_inpt(void)
{
	int			fd[2];
	pid_t		pid;
	int			status;
	char		*res;

	status = 0;
	if (pipe(fd) == -1)
		return (print_error(1, "pipe", "Cannot allocate memory"), NULL);
	set_signal_action(2);
	pid = fork();
	if (pid == -1)
		return (print_error(1, "pipe", "Cannot allocate memory"), NULL);
	if (!pid)
	{
		int	nbr;

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
		exit_code = WEXITSTATUS(status);
		return (NULL);
	}
	set_signal_action(0);
	close(fd[1]);
	res = get_next_line(fd[0]);
	close(fd[0]);
	return (res);
}

int	end_pipe_handler(t_token **last_token, char **first_inpt, char **env)
{
	char	*inpt;
	char	*replace_inpt;
	t_token	*token;

	replace_inpt = NULL;
	inpt = get_inpt();
	if (!inpt)
		return (0);
	token = lexer(inpt, env);
	if (!token || !check_token(&token, &inpt, env))
	{
		if (token)
			free_token(&token);
		return (0);
	}
	(*last_token)->next = token;
	token->prev = (*last_token);
	replace_inpt = join_inpts(*first_inpt, inpt);
	free(inpt);
	if (replace_inpt)
	{
		free(*first_inpt);
		(*first_inpt) = replace_inpt;
	}
	return (1);
}
