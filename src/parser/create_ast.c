/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:18:52 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/13 11:21:08 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*find_first_word_to_the_left(t_token *current)
{
	while (current->explored != 1)
	{
		if (current->prev && current->prev->type == 0 && current->explored == 0)
			current = current->prev;
		else
			break ;
	}
	return (current);	
}

t_token	*search_type(t_token **list, t_token *current, int type, int side)
{
	if (!list)
		return (NULL);
	if (!current)
		current = *list;
	while (current->explored != 1)
	{
		if (type < 2 && current->type == type && current->explored == 0)
		{
			if (side == 0 || type == 1)
				return (current);
			else
				return(find_first_word_to_the_left(current));
		}
		else if (type >= 2 && current->type >= 2 && current->explored == 0)
			return (current);
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
	{
		*error = 1;
		return (NULL);
	}
	current->explored = 1;
	node->type = PIPE;
	node->content = NULL;
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

t_ast	*create_ast(t_token **tokens, t_token *current, int after_explored, int *error)
{
	t_token	*check;
	t_ast	*node;

	//printf("error == %d\n", *error);
	//sleep(1);
	node = NULL;
	if ((check = search_type(tokens, current, PIPE, after_explored)) && *error == 0)
	{
		//printf("on trouve un pipe\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = pipe_node(tokens, check, error);
	}
	else if ((check = search_type(tokens, current, 2, after_explored)) && *error == 0)
	{
		//printf("on trouve une redir\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = redir_node(tokens, check, error);
	}
	else if ((check = search_type(tokens, current, WORD, after_explored)) && *error == 0)
	{
		//printf("on trouve un mot\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = word_node(check, error);
	}
	//printf("on check error\n");
	if (*error == 1)
	{
		//printf("error == 1 donc on free ce qu'il faut");
		return (free_error_node(node), NULL);
	}
	//printf("error == 0\n");
	return (node);
}
