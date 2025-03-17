/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:18:52 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/17 14:03:48 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_redir(t_token *current, int side)
{
	t_token	*check;

	check = current;
	if (side == 1)
	{
		while (check->explored != 1)
		{
			if (check->type >= 2)
				current = check;
			if (check->prev)
				check = check->prev;
			else
				break ;
		}
	}
	return (current);
}

t_token	*find_heredoc(t_token *current, int side)
{
	t_token	*check;

	check = current;
	if (side == 1)
	{
		while (check->explored != 1)
		{
			if (check->type == 6)
				current = check;
			if (check->prev)
				check = check->prev;
			else
				break ;
		}
	}
	return (current);
}

t_token	*search_type(t_token **list, t_token *current, int type, int side)
{
	if (!list)
		return (NULL);
	if (!current)
		current = *list;
	while (current)
	{
		if (current->explored == 1)
			break ;
		if (type < 2 && current->type == type && current->explored == 0)
			return (good_cur(current, side, type));
		else if (type == R_HEREDOC && current->type == R_HEREDOC
			&& current->explored == 0)
			return (find_heredoc(current, side));
		else if ((type >= 2 && type != R_HEREDOC) && current->type >= 2
			&& current->explored == 0)
			return (find_redir(current, side));
		if (side == 0 && current->next)
			current = current->next;
		else if (side == 1 && current->prev)
			current = current->prev;
		else
			break ;
	}
	return (NULL);
}

t_ast	*pipe_node(t_token **tokens, t_token *current, int *error)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (put_error_to_one(error), NULL);
	current->explored = 1;
	node->type = PIPE;
	node->content = NULL;
	node->arg = NULL;
	node->top = NULL;
	node->left = NULL;
	node->right = NULL;
	node->left = create_ast(tokens, current->prev, 1, error);
	if (*error == 1)
		return (free_error_node(node), NULL);
	if (node->left)
		node->left->top = node;
	node->right = create_ast(tokens, current->next, 0, error);
	if (*error == 1)
		return (free_error_node(node), NULL);
	if (node->right)
		node->right->top = node;
	return (node);
}

t_ast	*create_ast(t_token **tkn, t_token *cur, int side, int *error)
{
	t_ast	*node;

	node = NULL;
	if (search_type(tkn, cur, PIPE, side) && *error == 0)
		node = pipe_node(tkn, search_type(tkn, cur, PIPE, side), error);
	else if (search_type(tkn, cur, 2, side) && *error == 0)
		node = redir_node(tkn, search_type(tkn, cur, 2, side), error);
	else if (search_type(tkn, cur, WORD, side) && *error == 0)
		node = word_node(search_type(tkn, cur, WORD, side), error);
	if (*error == 1)
		return (free_error_node(node), NULL);
	if (node)
		node->done = 0;
	return (node);
}
