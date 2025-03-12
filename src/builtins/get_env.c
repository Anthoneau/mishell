/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:50:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/12 17:49:52 by agoldber         ###   ########.fr       */
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
	// printf("list->key : %s\n", list->key);
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
	int		len;
	int		i;

	if (!list || !list->head)
		return (NULL);
	ptr = list->head;
	i = 0;
	len = list_len(list);
	// printf(">>%d && %d\n", len, list->size);
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (NULL);
	while (ptr)
	{
		if (ptr->value)
		{
			tab[i] = fill_tab(ptr);
			// printf("tab[%d] : %s\n", i, tab[i]);
			if (!tab[i])
				return (free_array(tab), NULL);
			i++;
		}
		ptr = ptr->next;
	}
	tab[i] = NULL;
	return (tab);
}
