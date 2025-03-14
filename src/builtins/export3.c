/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:56:51 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/14 17:23:34 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_struct(t_tab ***arr)
{
	int	i;

	i = 0;
	while ((*arr)[i])
	{
		if ((*arr)[i]->key)
			free((*arr)[i]->key);
		if ((*arr)[i]->value)
			free((*arr)[i]->value);
		if ((*arr)[i])
			free((*arr)[i]);
		i++;
	}
	if ((*arr))
		free((*arr));
	(*arr) = NULL;
}

void	print_export(t_list *list, int output)
{
	int	i;

	sort(list->arr, list->size);
	i = 0;
	while (list->arr[i])
	{
		if (!list->arr[i]->value)
		{
			ft_putstr_fd("declare -x ", output);
			ft_putendl_fd(list->arr[i]->key, output);
			i++;
		}
		else
		{
			ft_putstr_fd("declare -x ", output);
			ft_putstr_fd(list->arr[i]->key, output);
			ft_putchar_fd('=', output);
			ft_putendl_fd(list->arr[i]->value, output);
			i++;
		}
	}
}

int	export_order(t_list *list, int output)
{
	int		i;
	t_node	*curr;

	i = 0;
	curr = list->head;
	list->arr = malloc(sizeof(t_tab *) * (list->size + 1));
	if (!list->arr)
		return (1);
	while (curr)
	{
		list->arr[i] = malloc(sizeof(t_tab));
		if (!list->arr[i])
			return (1);
		list->arr[i]->key = ft_strdup(curr->key);
		if (!list->arr[i]->key)
			return (free_struct(&list->arr), 1);
		list->arr[i]->value = ft_strdup(curr->value);
		if (!list->arr[i++]->value && curr->value)
			return (free_struct(&list->arr), 1);
		curr = curr->next;
	}
	list->arr[i] = NULL;
	print_export(list, output);
	free_struct(&list->arr);
	return (0);
}

int	check_key(t_tab *arr)
{
	int	i;

	i = 0;
	if (ft_isdigit(arr->key[0]))
		return (1);
	while (arr->key[i])
	{
		if (!ft_isalnum(arr->key[i]) && !(arr->key[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}
