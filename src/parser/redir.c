/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:15:02 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/13 12:44:14 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_content(t_token *current, t_ast *node, int *error)
{
	if (current->type == R_INPUT && current->prev)
	{
		node->content = ft_strdup(current->prev->content);
		if (!node->content)
			*error = 1;
		current->prev->explored = 1;
	}
	else if (current->next)
	{
		node->content = ft_strdup(current->next->content);
		if (!node->content)
			*error = 1;
		current->next->explored = 1;
	}
	else
		node->content = NULL;
}

int	create_content_and_sides(t_token **tokens, t_token *current, t_ast *node, int *error)
{
	create_content(current, node, error);
	if (current->type == R_INPUT)
	{
		node->left = create_ast(tokens, current->prev->prev, 1, error);
		node->right = create_ast(tokens, current->next, 0, error);
	}
	else
	{
		node->left = create_ast(tokens, current->prev, 1, error);
		node->right = create_ast(tokens, current->next->next, 0, error);
	}
	if (*error == 1)
		return (0);
	if (node && node->left)
		node->left->top = node;
	if (node && node->right)
	node->right->top = node;
	return (1);
}

t_ast	*redir_node(t_token **tokens, t_token *current, int *error)
{
	t_ast	*node;

	node = NULL;
	current->explored = 1;
	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	node->type = current->type;
	node->content = NULL;
	node->top = NULL;
	node->left = NULL;
	node->right = NULL;
	if (!current || !create_content_and_sides(tokens, current, node, error))
	{
		*error = 1;
		return (free_error_node(node), NULL);
	}
	return (node);
}
