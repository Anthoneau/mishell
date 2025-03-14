/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:02:57 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/14 15:07:09 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_unset(t_list *list, t_node *ptr)
{
	t_node	*tmp;

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
	int		i;
	t_node	*ptr;

	i = 0;
	while (arg[i])
	{
		ptr = get_node(list, arg[i]);
		if (ptr)
			do_unset(list, ptr);
		i++;
	}
	return (0);
}
