/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:08:02 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/13 18:40:51 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_char(t_list *list, char *s)
{
	t_node	*ptr;
	int		cmp;

	ptr = list->head;
	while (ptr != NULL)
	{
		cmp = ft_strncmp(s, ptr->key, ft_strlen(s));
		if (cmp == 0)
			return (ft_strdup(ptr->value));
		ptr = ptr->next;
	}
	return (NULL);
}

char	*get_value(char *str, bool button)
{
	int		len;
	char	*s;

	s = NULL;
	len = strllen(str, '=');
	if (str[len - 1] == '+')
		len--;
	if (button == false)
		s = ft_strldup(str, len);
	else
	{
		if (str[len] == '+')
			len++;
		if (str[len + 1] == '\0')
			return (ft_strdup(""));
		s = ft_strldup(str + (len + 1), ft_strlen(str + (len + 1)));
	}
	return (s);
}

void	get_list(t_list *list, char *str)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return ;
	new_node->key = fill_value(str, false);
	if (!new_node->key)
		return (free(new_node));
	new_node->value = fill_value(str, true);
	if (!new_node->value)
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

void	init_list(t_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->pwd = NULL;
	list->oldpd = NULL;
	list->arr = NULL;
	list->size = 0;
}

t_list	*make_list(char **env)
{
	int		i;
	t_list	*list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	init_list(list);
	i = 0;
	while (env[i])
	{
		get_list(list, env[i]);
		if (!list->head || !list->tail)
			return (free_list(list), NULL);
		i++;
	}
	list->pwd = get_char(list, "PWD");
	if (!list->pwd)
		return (free_list(list), NULL);
	list->oldpd = get_char(list, "OLDPWD");
	return (list);
}
