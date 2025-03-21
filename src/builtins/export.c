/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:02:27 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/20 14:48:52 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_value(t_node *curr, t_tab *arr)
{
	char	*tmp;

	if (!arr->value)
		return (0);
	if (arr->lever == true)
	{
		if (curr->value)
		{
			tmp = ft_strjoin(curr->value, arr->value);
			free(curr->value);
			curr->value = tmp;
		}
		else
			curr->value = ft_strdup(arr->value);
	}
	else
	{
		if (curr->value)
			free(curr->value);
		curr->value = ft_strdup(arr->value);
	}
	if (!curr->value && arr->value)
		return (1);
	return (0);
}

void	print_error_export(char *arg)
{
	print_e(1, "export", 0, NULL);
	ft_putchar_fd('`', 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("\': not a valid identifier", 2);
}

int	fill_arr(t_tab ***arr, int i, char **arg)
{
	(*arr)[i]->key = get_value(arg[i], false);
	if (!(*arr)[i]->key)
		return (free_struct(arr), 1);
	(*arr)[i]->value = get_value(arg[i], true);
	if (!(*arr)[i]->value)
		return (free_struct(arr), 1);
	return (0);
}

int	do_export(t_list **list, int output, char **arg)
{
	(*list)->error = 0;
	if (arg[0] == NULL && export_order(*list, output))
		return (1);
	return (0);
}

int	export(t_list *list, char **arg, int output)
{
	int		i;
	t_node	*ptr;

	output = get_output(output);
	i = 0;
	if (do_export(&list, output, arg) || tab_fill(&list->arr, arg, list))
		return (free_list(list), 1);
	while (list->arr[i])
	{
		if (list->arr[i]->key)
		{
			ptr = check_env(list, list->arr[i]->key);
			if (!ptr && check_key(list->arr[i]))
				return (print_error_export(arg[i]), 1);
			else if (!ptr)
				export_add(list, list->arr[i]);
			else if (change_value(ptr, list->arr[i]))
				return (free_list(list), free_struct(&list->arr), 1);
		}
		i++;
	}
	if (list->error)
		return (free_struct(&list->arr), 1);
	return (free_struct(&list->arr), 0);
}
