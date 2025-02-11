/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:16:54 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 11:36:48 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_len(t_token *current)
{
	t_token	*words;
	size_t	len;

	len = 0;
	words = current;
	while (words)
	{
		if (words->type == WORD && words->content
			&& words->explored == 0 && !(words->prev
				&& words->prev->type >= R_INPUT))
			len++;
		if (words->next && words->next->type != PIPE)
			words = words->next;
		else
			break ;
	}
	return (len + 1);
}

char	**get_word(t_token *current)
{
	char	**res;
	t_token	*words;
	size_t	len;
	int		i;

	if (!current || !current->content)
		return (NULL);
	words = current;
	len = get_len(current);
	res = ft_calloc(len, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (words)
	{
		if (words->type == WORD && words->content
			&& words->explored == 0 && !(words->prev
				&& words->prev->type >= R_INPUT))
		{
			res[i] = ft_strdup(words->content);
			words->explored = 1;
			i++;
		}
		if (words->next && words->next->type != PIPE)
			words = words->next;
		else
			break ;
	}
	return (res);
}

t_ast	*word_node(t_token *current, int *error)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	node->type = WORD;
	node->content = NULL;
	node->arg = get_word(current);
	if (!node->arg)
	{
		*error = 1;
		return (free(node), NULL);
	}
	node->top = NULL;
	node->right = NULL;
	node->left = NULL;
	current->explored = 1;
	return (node);
}
