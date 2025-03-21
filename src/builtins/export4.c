/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:50:44 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/20 14:38:08 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hein(char **arg, int i, t_tab ***arr)
{
	int	len;

	if (ft_strchr(arg[i], '+'))
	{
		len = strllen(arg[i], '=');
		if (arg[i][len - 1] == '+')
			(*arr)[i]->lever = true;
	}
	(*arr)[i]->key = get_value(arg[i], false);
	if (!(*arr)[i]->key)
		return (1);
	(*arr)[i]->value = get_value(arg[i], true);
	if (!(*arr)[i]->value)
		return (1);
	return (0);
}

int	yo(t_tab ***arr, int i, char **arg, t_list *list)
{
	(*arr)[i]->lever = false;
	if (!ft_strchr(arg[i], '='))
	{
		(*arr)[i]->key = get_value(arg[i], false);
		if (!(*arr)[i]->key)
			return (1);
		(*arr)[i]->value = NULL;
	}
	else if ((ft_isalpha(arg[i][0]) || arg[i][0] == '_'))
	{
		if (hein(arg, i, arr))
			return (1);
	}
	else
	{
		print_error_export(arg[i]);
		list->error = 1;
		(*arr)[i]->key = NULL;
		(*arr)[i]->value = get_value(arg[i], true);
		if (!(*arr)[i]->value)
			return (1);
	}
	return (0);
}

int	tab_fill(t_tab ***arr, char **arg, t_list *list)
{
	int	i;
	int	count;

	i = 0;
	count = word_count(arg);
	(*arr) = malloc(sizeof(t_tab *) * (count + 1));
	if (!(*arr))
		return (1);
	while (arg[i])
	{
		(*arr)[i] = malloc(sizeof(t_tab));
		if (!(*arr)[i])
			return (free_struct(arr), 1);
		if (yo(arr, i, arg, list))
			return (free_struct(arr), 1);
		i++;
	}
	(*arr)[i] = NULL;
	return (0);
}
