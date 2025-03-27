/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:44:38 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/25 14:57:11 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *c)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token", 2);
	ft_putstr_fd(" `", 2);
	ft_putstr_fd(c, 2);
	ft_putendl_fd("'", 2);
}

void	solo_h(t_token *current)
{
	if (current->type != PIPE && current->type != PIPE2
		&& current->type != AND && current->type != AND2
		&& current->type != DOT && current->type != DOT2)
		print_syntax_error("newline");
	else if (current->type == PIPE)
		print_syntax_error("|");
	else if (current->type == PIPE2)
		print_syntax_error("||");
	else if (current->type == AND)
		print_syntax_error("&");
	else if (current->type == AND2)
		print_syntax_error("&&");
	else if (current->type == DOT)
		print_syntax_error(";");
	else if (current->type == DOT2)
		print_syntax_error(";;");
}

void	error_h(t_token *current)
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
	else if (current->next->type == PIPE2)
		print_syntax_error("||");
	else if (current->next->type == AND)
		print_syntax_error("&");
	else if (current->next->type == AND2)
		print_syntax_error("&&");
	else if (current->next->type == DOT)
		print_syntax_error(";");
	else if (current->next->type == DOT2)
		print_syntax_error(";;");
}

int	forbidden_token(t_token **token)
{
	t_token	*current;

	current = *token;
	while (current)
	{
		if (current->type == R_INPUT_TRUC)
			return (print_e(1, NULL, 0, "unknown token `<>'"), 1);
		if (current->type == PIPE2)
			return (print_e(1, NULL, 0, "unknown token `||'"), 1);
		if (current->type == AND)
			return (print_e(1, NULL, 0, "unknown token `&'"), 1);
		if (current->type == AND2)
			return (print_e(1, NULL, 0, "unknown token `&&'"), 1);
		if (current->type == DOT)
			return (print_e(1, NULL, 0, "unknown token `;'"), 1);
		if (current->type == DOT2)
			return (print_e(1, NULL, 0, "unknown token `;;'"), 1);
		if (current->next)
			current = current->next;
		else
			break ;
	}
	return (0);
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
