/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:00:08 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/18 12:08:08 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_shell(t_list *list)
{
	t_tab	arr;

	arr.key = "SHLVL";
	arr.value = "1";
	arr.lever = false;
	export_add(list, &arr);
	return (0);
}

int	is_nbr(char	*s)
{
	int	i;

	i = 0;
	while (s[i] == 32 || s[i] == '-')
		i++;
	while (s[i] && ft_isdigit(s[i]))
		i++;
	while (s[i])
	{
		if (s[i] != 32)
			return (0);
		i++;
	}
	return (1);
}

int	shell_nbr(t_node *ptr)
{
	int	res;

	res = ft_atoi(ptr->value);
	if (res < 0)
	{
		if (fill_shell_value(ptr, "0"))
			return (1);
		return (0);
	}
	res++;
	if (res <= 999)
	{
		free(ptr->value);
		ptr->value = ft_itoa(res);
		if (!ptr->value)
			return (1);
		return (0);
	}
	printf("minishell: warning: shell level ");
	printf("(%d) too high, resetting to 1\n", res);
	if (fill_shell_value(ptr, "1"))
		return (1);
	return (0);
}

int	replace_shell(t_node *ptr)
{
	if (!is_nbr(ptr->value))
	{
		if (fill_shell_value(ptr, "1"))
			return (1);
		return (0);
	}
	if (shell_nbr(ptr))
		return (1);
	return (0);
}

int	init_shelvl(t_list *list)
{
	t_node	*ptr;

	ptr = get_node(list, "SHLVL");
	if (ptr)
	{
		if (replace_shell(ptr))
			return (1);
		return (0);
	}
	add_shell(list);
	return (0);
}
