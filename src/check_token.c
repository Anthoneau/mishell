/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:10:55 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/23 15:28:27 by agoldber         ###   ########.fr       */
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

// void	join_token_to_list(t_token **last_token, t_token *token_to_add)
// {
	// t_token	*current;

	// current = *list;
	// while (current)
	// {
	// 	if (current->next)
	// 		current = current->next;
	// 	else
	// 		break ;
	// }
	// current->next = token_to_add;
	// token_to_add->prev = current;
// }

void	print_error_inpt(void)
{
	printf("minishell : error joining inputs\n");
}

char	*join_inpts(char *first_inpt, char *inpt)
{
	int		i;
	char	*res;
	char	*temp;

	i = ft_strlen(first_inpt);
	// printf("first_inpt : %s\nstrlen de first_inpt : %d\n", first_inpt, i);
	res = NULL;
	temp = NULL;
	if (inpt[i] == ' ')
	{
		// printf("on trouve un espace a inpt[%d] [%c]\n", i, inpt[i]);
		res = ft_strjoin(first_inpt, inpt);
		// printf("res (join) : %s\n", res);
	}
	else
	{
		// printf("on trouve pas d'espace\n");
		temp = ft_strjoin(first_inpt, " ");
		if (!temp)
			return (print_error_inpt(), NULL);
		// printf("temp (join 1) : %s\n", temp);
		res = ft_strjoin(temp, inpt);
		// printf("res (join 2) : %s\n", res);
		free(temp);
	}
	if (!res)
		return (print_error_inpt(), NULL);
	return (res);
}

int	pipe_handler(t_token **last_token, char **first_inpt, char **env)
{
	char	*inpt;
	char	*replace_inpt;
	t_token	*token;

	replace_inpt = NULL;
	inpt = readline("pipe > ");
	
	token = lexer(inpt);
	if (!token || !check_token(&token, &inpt, env))
	{
		return (0);
	}

	// join_token_to_list(last_token, token);
	(*last_token)->next = token;
	token->prev = (*last_token);
	replace_inpt = join_inpts(*first_inpt, inpt);
	// printf("replace inpt : %s\n", replace_inpt);
	free(inpt);
	if (replace_inpt)
	{
		// printf("on free first_inpt\n");
		free(*first_inpt);
		(*first_inpt) = replace_inpt;
		// printf("on assigne replace_inpt a first_inpt\n");
		// printf("new inpt : %s\n", *first_inpt);
	}
	return (1);
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
		else if (current->type == PIPE && !pipe_handler(&current, inpt, env))
			return (0);// pipe_handler(token, env); // a faire
		else
			break ;
	}
	if (forbidden_token(token))
		return (0);
	// printf("retour dans check_token\ninpt : %s\n", *inpt);
	return (1);
}
