/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:50:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/12 19:33:45 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	list_len(t_list *list)
{
	t_node	*ptr;
	int		i;

	i = 0;
	ptr = list->head;
	while (ptr)
	{
		if (ptr->value != NULL)
			i++;
		ptr = ptr->next;
	}
	return (i);
}

char	*fill_tab(t_node *list)
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

char	**get_env(t_list *list)
{
	t_node	*ptr;
	char	**tab;
	int		i;

	if (!list || !list->head)
		return (NULL);
	ptr = list->head;
	i = 0;
	tab = malloc(sizeof(char *) * (list_len(list) + 1));
	if (!tab)
		return (NULL);
	while (ptr)
	{
		if (ptr->value)
		{
			tab[i] = fill_tab(ptr);
			if (!tab[i])
				return (free_array(tab), NULL);
			i++;
		}
		ptr = ptr->next;
	}
	tab[i] = NULL;
	return (tab);
}
