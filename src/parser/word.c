/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:16:54 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/13 11:18:02 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	get_len(t_token *current)
{
	t_token *words;
	size_t	len;

	len = 0;
	words = current;
	while (words->type == 0 && words->content)
	{
		len += ft_strlen(words->content) + 1;
		if (words->next && words->next->type == 0 && words->next->content)
			words = words->next;
		else
			break ;
	}
	return (len);
}

char	*get_word(t_token *current)
{
	char	*res;
	t_token	*words;
	size_t	len;

	if (!current || !current->content)
		return (NULL);
	words = current;
	len = get_len(current);
	res = ft_calloc(len, sizeof(char));
	if (!res)
		return (NULL);
	while (words->type == 0 && words->content)
	{
		ft_strcat(words->content, res);
		if (words->next && words->next->type == 0 && words->next->content)
			words = words->next;
		else
			break ;
	}
	return (res);
}

t_ast	*word_node(t_token *current, int *error)
{
	t_ast	*node;

	//printf("\n---------- WORD NODE ----------\n");
	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	node->type = WORD;
	node->content = get_word(current);
	//printf("content : %s\n", node->content);
	//sleep(1);
	if (!node->content)
	{
		*error = 1;
		return (free(node), NULL);
	}
	node->top = NULL;
	node->right = NULL;
	node->left = NULL;
	current->explored = 1;
	//printf("noeud initialisé\n");
	//printf("\n-------------------------------\n");
	return (node);
}
