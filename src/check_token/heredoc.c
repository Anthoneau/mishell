/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:47:05 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/12 19:25:39 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	modify_inpt(char **inpt, char **env)
{
	if (!*inpt)
		return (0);
	else if (expandable(*inpt))
		to_expand(inpt, env);
	if (contain_backslash(*inpt))
		del_backslash(*inpt);
	if (!*inpt)
		return (0);
	return (1);
}

void	print_eof(char *delimiter, int line)
{
	print_e(1, "warning", 0, NULL);
	ft_putstr_fd("here-document at line ", 2);
	ft_putnbr_fd(line, 2);
	ft_putstr_fd(" delimited ", 2);
	ft_putstr_fd("by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

int	heredoc(char *delimiter, int expand, char **env, int fd[2])
{
	char	*inpt;
	int		line;

	inpt = NULL;
	line = 0;
	while (1)
	{
		inpt = readline("heredoc > ");
		if (!inpt)
		{
			close(fd[1]);
			close(fd[0]);
			return (print_eof(delimiter, line), -1);
		}
		if (!(ft_strncmp(delimiter, inpt, ft_strlen(delimiter) + 1)))
			break ;
		if (expand && !modify_inpt(&inpt, env))
			return (print_e(1, "malloc", 1, ""), -1);
		ft_putendl_fd(inpt, fd[1]);
		free(inpt);
		line++;
	}
	close(fd[1]);
	free(inpt);
	return (0);
}

int	heredoc_child(pid_t pid, t_token *cur, char **env, int fd[2])
{
	int			status;
	int			nbr;
	extern int	g_exit_code;

	status = 0;
	if (!pid)
	{
		nbr = 0;
		set_signal_action(1);
		nbr = heredoc(cur->next->content, cur->next->expand, env, fd);
		close(fd[1]);
		close(fd[0]);
		exit(nbr);
	}
	waitpid(pid, &status, 0);
	if (status != 0)
	{
		close(fd[1]);
		close(fd[0]);
		g_exit_code = WEXITSTATUS(status);
		return (0);
	}
	return (1);
}

int	do_heredoc(t_token *cur, char **env)
{
	pid_t	pid;
	int		fd[2];

	pid = 0;
	if (pipe(fd) == -1)
		return (print_e(1, "heredoc", 1, ""), 0);
	set_signal_action(2);
	pid = fork();
	if (pid == -1)
		return (print_e(1, "heredoc", 1, ""), 0);
	if (!heredoc_child(pid, cur, env, fd))
		return (0);
	set_signal_action(0);
	close(fd[1]);
	cur->fd = fd[0];
	if (cur->fd == -1)
		return (0);
	return (1);
}
