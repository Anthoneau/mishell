/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:50:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/05 17:51:59 by mel-bout         ###   ########.fr       */
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
	int		len;
	int		i;

	if (!list || !list->head)
		return (NULL);
	ptr = list->head;
	i = 0;
	len = list_len(list);
	printf(">>%d\n", len);
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (NULL);
	while (ptr)
	{
		tab[i] = fill_tab(ptr);
		if (!tab[i])
			return (free_array(tab), NULL);
		i++;
		ptr = ptr->next;
	}
	tab[i] = NULL;
	return (tab);
}