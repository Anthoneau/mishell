/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:10:55 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/22 17:50:03 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *c)
{
	printf("minishell: syntax error near unexpected token `%s'\n", c);
}

// void	err_trunc_handler(t_token *current)
// {
// 	current = current->next;
// 	if (current->type == PIPE)
// 	{
// 		if (current->next && current->next->type == PIPE)
// 		{
// 			current = current->next;
// 			if (current->next && current->next->type == PIPE)
// 			{
// 				print_syntax_error("||");
// 			}
// 			else
// 			{
// 				print_syntax_error("|");
// 			}
// 		}
// 		else
// 		{
// 			if (current->next && current->next->type == R_TRUNC)
// 			{
// 				print_syntax_error(">");
// 			}
// 			else
// 			{
// 				print_syntax_error("newline");
// 			}
// 		}
// 	}
// }

// void	error_handler(t_token *current)
// {
// 	// if (current->type == R_INPUT)
// 	// {
// 	// 	err_input_handler();
// 	// }
// 	if (current->type == R_TRUNC)
// 	{
// 		err_trunc_handler(current);
// 	}
// 	// else if (current->type == R_HEREDOC)
// 	// {
// 	// 	err_heredoc_handler();
// 	// }
// 	// else if (current->type == R_APPEND)
// 	// {
// 		// err_append_handler();
// 	// }
// }

void	solo_handler(t_token *current)
{
	if (current->type != PIPE)
		print_syntax_error("newline");
	else
		print_syntax_error("|");
}

void	error_handler(t_token *current)
{
	if (current->next->type == R_INPUT)
		print_syntax_error("<");
	else if (current->next->type == R_INPUT_TRUC)
		print_syntax_error("<>");
	else if (current->next->type == R_TRUNC)
		print_syntax_error(">");
	else if (current->next->type == R_TRUNC_NOCLOBBER)
		print_syntax_error(">|");
	else if (current->next->type == R_APPEND)
		print_syntax_error(">>");
	else if (current->next->type == R_HEREDOC)
		print_syntax_error("<<");
	else if (current->next->type == PIPE)
		print_syntax_error("|");
}

void	heredoc(void)
{
	char	*inpt;

	inpt = readline("heredoc > ");
	free(inpt);
}

void	pipe_handler(t_token **token)
{
	char	*inpt;

	inpt = readline("pipe > ");
	(void)token;
	free(inpt);
}

int	forbidden_token(t_token **token)
{
	t_token *current;

	current = *token;
	while (current)
	{
		if (current->type == R_INPUT_TRUC)
		{
			printf("minishell: unknown token `<>'\n");
			return (1);
		}
		if (current->next)
			current = current->next;
		else
			break ;
	}
	return (0);
}

int	check_token(t_token **token)
{
	t_token	*current;

	current = *token;
	if ((current->type != WORD && !current->next) || current->type == PIPE)
		return (solo_handler(current), 0);
	while (current)
	{
		if (current->next && ((current->type != WORD && current->type != PIPE && current->next->type != WORD) || (current->type == PIPE && current->next->type == PIPE)))
			return (error_handler(current), 0);
		if (current->type == R_HEREDOC)
			heredoc(); // a faire
		if (current->next)
			current = current->next;
		else if (current->type == PIPE)
			pipe_handler(token); // a faire
		else
			break ;
	}
	if (forbidden_token(token))
		return (0);
	return (1);
}
