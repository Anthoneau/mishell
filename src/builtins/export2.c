/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:53:30 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/13 19:02:03 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*fill_export(t_node *list)
{
	char	*key;
	char	*str;

	if (!list || !list->key)
		return (NULL);
	if (!list->value)
		return (ft_strdup(list->key));
	key = ft_strjoin(list->key, "=");
	if (!key)
		return (NULL);
	str = ft_strjoin(key, list->value);
	free(key);
	if (!str)
		return (NULL);
	return (str);
}

void	export_add(t_list *list, t_tab *arr)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(arr->key);
	if (!new_node->key)
		return (free(new_node));
	new_node->value = ft_strdup(arr->value);
	if (!new_node->value && arr->value)
		return (free(new_node->key), free(new_node));
	new_node->next = NULL;
	new_node->prev = NULL;
	if (list->tail != NULL)
	{
		new_node->prev = list->tail;
		list->tail->next = new_node;
	}
	list->tail = new_node;
	list->size++;
	if (list->head == NULL)
		list->head = new_node;
}

char	*fill_value(char *str, bool button)
{
	if (ft_strchr(str, '='))
		return (get_value(str, button));
	else
	{
		if (button == false)
			return (ft_strdup(str));
		else
			return (NULL);
	}
}

t_node	*check_env(t_list *list, char *str)
{
	t_node	*curr;

	curr = list->head;
	while (curr)
	{
		if (!ft_strncmp(str, curr->key, ft_strlen(str) + 1))
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

int	word_count(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}
