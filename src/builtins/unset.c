/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:02:57 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/12 20:15:58 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_unset(t_list *list, char *s)
{
	printf("unset\n");
	t_node  *ptr;
	t_node  *tmp;
	int     cmp;
	
	ptr = list->head;
	while (ptr != NULL)
	{
		cmp = ft_strncmp(s, ptr->key, ft_strlen(s) + 1);
		if (cmp == 0)
		{
			/////////////
			// printf("\n\nXXXXXXXXXXXXXXXX\n");
			// printf("ptr = <%s> ptr->prev = <%s>, ptr->next = %p\n", ptr->key, ptr->prev->key, ptr->next); 
			////////////
			if (ptr->prev != NULL)
			{
				tmp = ptr->prev;
				tmp->next = ptr->next;
				if (ptr->next == NULL)
					list->tail = tmp;
			}
			/////////////
			// printf("tmp = <%s> tmp->next = <%s>\n", tmp->key, tmp->next->key);
			// printf("{tail->next = NULL} tmp = <%s> tmp->next = %p\n", tmp->key, tmp->next);
			////////////
			if (ptr->next != NULL)
			{
				tmp = ptr->next;
				tmp->prev = ptr->prev;
				if (ptr->prev == NULL)
					list->head = tmp;
			}
			////////////
			// printf("tmp = <%s> tmp->prev = <%s>\n", tmp->key, tmp->prev->key);
			// printf("{head->prev = NULL} tmp = <%s> tmp->prev = %p\n", tmp->key, tmp->prev);
			// printf("XXXXXXXXXXXXXXXX\n\n");
			////////////
			if (ptr->key)
				free(ptr->key);
			if (ptr->value)
				free(ptr->value);
			free(ptr);
			list->size--;
			return ;
		}
		ptr = ptr->next;
	}
}

int	unset(t_list *list, char **arg)
{
	int	i;

	i = 0;
	while(arg[i])
	{
		do_unset(list, arg[i]);
		i++;
	}
	return (0);
}
