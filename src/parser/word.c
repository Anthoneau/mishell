/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:16:54 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/07 15:08:50 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_len(t_token *current)
{
	t_token *words;
	size_t	len;

	len = 0;
	words = current;
	while (words)
	{
		if (words->type == WORD && words->content && words->explored == 0 && !(words->prev && words->prev->type >= R_INPUT))
		{
			// len += ft_strlen(words->content) + 1;
			len++;
			// ft_strcat(words->content, res);
			// words->explored = 1;
		}
		// if (words->type == WORD && words->content)
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
	// char	*temp;
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
		if (words->type == WORD && words->content && words->explored == 0 && !(words->prev && words->prev->type >= R_INPUT))
		{
			// ft_strcat(words->content, res);
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
	/*
		Fonction pour creer les mots
		On initialise tout et on envoit get_word a content
	*/
	t_ast	*node;

	//printf("\n---------- WORD NODE ----------\n");
	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	node->type = WORD;
	// node->content = get_word(current);
	node->content = NULL;
	node->arg = get_word(current);
	// printf("content : %s\n", node->content);
	// sleep(1);
	if (!node->arg)
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
