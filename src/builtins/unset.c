/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:02:57 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/13 17:41:39 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	do_unset(t_list *list, char *s)
// {
// 	printf("unset\n");
// 	t_node  *ptr;
// 	t_node  *tmp;
// 	int     cmp;
	
// 	ptr = list->head;
// 	while (ptr != NULL)
// 	{
// 		cmp = ft_strncmp(s, ptr->key, ft_strlen(s) + 1);
// 		if (cmp == 0)
// 		{
// 			if (ptr->prev != NULL)
// 			{
// 				tmp = ptr->prev;
// 				tmp->next = ptr->next;
// 				if (ptr->next == NULL)
// 					list->tail = tmp;
// 			}
// 			if (ptr->next != NULL)
// 			{
// 				tmp = ptr->next;
// 				tmp->prev = ptr->prev;
// 				if (ptr->prev == NULL)
// 					list->head = tmp;
// 			}
// 			if (ptr->key)
// 				free(ptr->key);
// 			if (ptr->value)
// 				free(ptr->value);
// 			free(ptr);
// 			list->size--;
// 			return ;
// 		}
// 		ptr = ptr->next;
// 	}
// }

// int	unset(t_list *list, char **arg)
// {
// 	int	i;

// 	i = 0;
// 	while(arg[i])
// 	{
// 		do_unset(list, arg[i]);
// 		i++;
// 	}
// 	return (0);
// }


void	do_unset(t_list *list, t_node *ptr)
{
	t_node  *tmp;
	
	if (ptr->prev != NULL)
	{
		tmp = ptr->prev;
		tmp->next = ptr->next;
		if (ptr->next == NULL)
			list->tail = tmp;
	}
	if (ptr->next != NULL)
	{
		tmp = ptr->next;
		tmp->prev = ptr->prev;
		if (ptr->prev == NULL)
			list->head = tmp;
	}
	if (ptr->key)
		free(ptr->key);
	if (ptr->value)
		free(ptr->value);
	free(ptr);
	list->size--;
	return ;
}

int	unset(t_list *list, char **arg)
{
	int	i;
	t_node	*ptr;

	i = 0;
	while(arg[i])
	{
		ptr = get_node(list, arg[i]);
		if (ptr)
			do_unset(list, ptr);
		i++;
	}
	return (0);
}