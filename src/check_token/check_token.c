/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:10:55 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/23 15:47:00 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *c)
{
	printf("minishell: syntax error near unexpected token `%s'\n", c);
}

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

int	check_token(t_token **token, char **inpt, char **env)
{
	t_token	*current;

	current = *token;
	if ((current->type != WORD && !current->next) || current->type == PIPE)
		return (solo_handler(current), 0);
	while (current)
	{
		if (current->next && ((current->type != WORD && current->type != PIPE
			&& current->next->type != WORD) || (current->type == PIPE
			&& current->next->type == PIPE)))
			return (error_handler(current), 0);
		if (current->type == R_HEREDOC)
			heredoc(); // a faire
		if (current->next)
			current = current->next;
		else if (current->type == PIPE && !end_pipe_handler(&current, inpt, env))
			return (0);// pipe_handler(token, env); // a faire
		else
			break ;
	}
	if (forbidden_token(token))
		return (0);
	// printf("retour dans check_token\ninpt : %s\n", *inpt);
	return (1);
}
