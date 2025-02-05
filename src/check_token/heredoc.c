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

void	heredoc(char *delimiter, int expand)
{
	char	*inpt;
	int		pipe_buf[2];

	inpt = NULL;
	if (pipe(pipe_buf) == -1)
		return (print_error_message(1, "pipe", "Cannot allocate memory"));
	while (1)
	{
		inpt = readline("heredoc > ");
		if (!inpt)
		{
			close(pipe_buf[1]);
			close(pipe_buf[0]);
			return (print_error_message(1, "pipe", "Cannot allocate memory"));
		}
		if (!(ft_strncmp(delimiter, inpt, ft_strlen(delimiter) + 1)))
			break ;
		// if (expand)
		// {
			
		// }
		ft_putstr_fd(inpt, pipe_buf[1]);
		ft_putstr_fd("\n", pipe_buf[1]);
		free(inpt);
	}
	printf("inpt : %s\n", inpt);
	close(pipe_buf[1]);
	free(inpt);
}
