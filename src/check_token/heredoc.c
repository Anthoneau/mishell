/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:47:05 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 11:03:55 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	heredoc(char *delimiter, int expand, char **env)
{
	char	*inpt;
	int		fd[2];

	inpt = NULL;
	if (pipe(fd) == -1)
		return (print_error(1, "pipe", "Cannot allocate memory"), -1);
	while (1)
	{
		inpt = readline("heredoc > ");
		if (!inpt)
		{
			close(fd[1]);
			close(fd[0]);
			return (print_error(1, "malloc", "Cannot allocate memory"), -1);
		}
		if (!(ft_strncmp(delimiter, inpt, ft_strlen(delimiter) + 1)))
			break ;
		if (expand && !modify_inpt(&inpt, env))
			return (print_error(1, "malloc", "Cannot allocate memory"), -1);
		ft_putendl_fd(inpt, fd[1]);
		free(inpt);
	}
	close(fd[1]);
	free(inpt);
	return (fd[0]);
}

int	do_heredoc(t_token *cur, char **env)
{
	cur->fd = heredoc(cur->next->content, cur->next->expand, env);
	if (cur->fd == -1)
		return (0);
	return (1);
}
