/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:47:05 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/05 16:54:10 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(char *delimiter, int expand)
{
	char	*inpt;
	int		fd[2];

	inpt = NULL;
	if (pipe(fd) == -1)
		return (print_error_message(1, "pipe", "Cannot allocate memory"), -1);
	while (1)
	{
		inpt = readline("heredoc > ");
		if (!inpt)
		{
			close(fd[1]);
			close(fd[0]);
			return (print_error_message(1, "pipe", "Cannot allocate memory"), -1);
		}
		if (!(ft_strncmp(delimiter, inpt, ft_strlen(delimiter) + 1)))
			break ;
		if (expand)
		{
			printf("ah\n");
		}
		ft_putstr_fd(inpt, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(inpt);
	}
	printf("inpt : %s\n", inpt);
	close(fd[1]);
	free(inpt);
	return (fd[0]);
}
