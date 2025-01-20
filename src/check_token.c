/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:10:55 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/20 17:32:18 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *c)
{
	printf("minishell: syntax error near unexpected token `%s'\n", c);
}

void	err_trunc_handler(t_token *current)
{
	current = current->next;
	if (current->type == PIPE)
}

void	error_handler(t_token *current)
{
	if (current->type == R_INPUT)
	{
		err_input_handler();
	}
	else if (current->type == R_TRUNC)
	{
		err_trunc_handler(current);
	}
	else if (current->type == R_HEREDOC)
	{
		err_heredoc_handler();
	}
	else if (current->type == R_APPEND)
	{
		err_append_handler();
	}
}

int	check_token(t_token **token)
{
	t_token	*current;

	current = *token;
	while (current)
	{
		if (current->next)
		{
			if (current->type != R_INPUT && current->type != R_TRUNC)
			{
				if (current->type != WORD && current->next->type != WORD)
				{
					return (error_handler(current), 0);
				}
				else if (current->type != WORD && current->type != PIPE && !current->next)
				{
					return (error_end_handle(current), 0);
				}
				else if (current->type == R_HEREDOC)
				{
					create_heredoc(current);
				}
				else if (current->type == PIPE && !current->next)
				{
					return (end_pipe_handle(token, current));
				}
			}
			current = current->next;
		}
		else
			break ;
	}
	return (1);
}
