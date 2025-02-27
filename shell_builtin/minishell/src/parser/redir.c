/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:15:02 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 11:35:27 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_content(t_token *current, t_ast *node, int *error)
{
	if (current->next)
	{
		node->content = ft_strdup(current->next->content);
		if (!node->content)
			*error = 1;
		current->next->explored = 1;
	}
	else
		node->content = NULL;
}

int	create_cont_sides(t_token **tokens, t_token *cur, t_ast *node, int *error)
{
	create_content(cur, node, error);
	if (cur->type == R_HEREDOC)
	{
		node->fd = dup(cur->fd);
		close(cur->fd);
	}
	node->left = create_ast(tokens, cur->prev, 1, error);
	node->right = create_ast(tokens, cur->next->next, 0, error);
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
	node->arg = NULL;
	node->top = NULL;
	node->left = NULL;
	node->right = NULL;
	if (!current || !create_cont_sides(tokens, current, node, error))
	{
		*error = 1;
		return (free_error_node(node), NULL);
	}
	return (node);
}
