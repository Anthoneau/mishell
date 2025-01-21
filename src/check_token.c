/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:10:55 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/21 17:12:03 by agoldber         ###   ########.fr       */
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
	{
		if (current->next && current->next->type == PIPE)
		{
			current = current->next;
			if (current->next && current->next->type == PIPE)
			{
				print_syntax_error("||");
			}
			else
			{
				print_syntax_error("|");
			}
		}
		else
		{
			if (current->next && current->next->type == R_TRUNC)
			{
				print_syntax_error(">");
			}
			else
			{
				print_syntax_error("newline");
			}
		}
	}
}

void	error_handler(t_token *current)
{
	// if (current->type == R_INPUT)
	// {
	// 	err_input_handler();
	// }
	if (current->type == R_TRUNC)
	{
		err_trunc_handler(current);
	}
	// else if (current->type == R_HEREDOC)
	// {
	// 	err_heredoc_handler();
	// }
	// else if (current->type == R_APPEND)
	// {
		// err_append_handler();
	// }
}

void	pipe_handler(t_token **token)
{
	char	*inpt;

	inpt = readline("pipe > ");
	(void)token;
	free(inpt);
}

void	solo_handler(t_token *current)
{
	if (current->type != PIPE)
		print_syntax_error("newline");
	else
		print_syntax_error("|");
}

void	heredoc(void)
{
	printf("heredoc\n");
}

int	check_token(t_token **token)
{
	t_token	*current;

	current = *token;
	if (!current->next && current->type != WORD)
		return (solo_handler(current), 0);
	while (current)
	{
		if (current->type == R_HEREDOC)
			heredoc();
		if (current->next)
		{
			if (current->type != R_INPUT && current->next->type != R_TRUNC)
			{
				if (current->type != WORD && current->next->type != WORD)
				{
					return (error_handler(current), 0);
				}
				// else if (current->type != WORD && current->type != PIPE && !current->next)
				// {
				// 	return (error_end_handle(current), 0);
				// }
				// else if (current->type == R_HEREDOC)
				// {
				// 	create_heredoc(current);
				// }
				// else if (current->type == PIPE && !current->next)
				// {
				// 	return (end_pipe_handle(token, current));
				// }
			}
			current = current->next;
		}
		else if (current->type != PIPE)
		{
			return (solo_handler(current), 0);
		}
		else if (current->type == PIPE && !current->next)
		{
			pipe_handler(token);
		}
		else
			break ;
	}
	return (1);
}
