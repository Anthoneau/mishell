/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:02:27 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/13 19:02:39 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*fill_export(t_node *list)
// {
// 	char	*key;
// 	char	*str;

// 	if (!list || !list->key)
// 		return (NULL);
// 	if (!list->value)
// 		return (ft_strdup(list->key));
// 	key = ft_strjoin(list->key, "=");
// 	if (!key)
// 		return (NULL);
// 	str = ft_strjoin(key, list->value);
// 	free(key);
// 	if (!str)
// 		return (NULL);
// 	return (str);
// }

// // char	**export_arr(t_list *list)
// // {
// // 	t_node	*ptr;
// // 	char	**tab;
// // 	int		i;

// // 	if (!list || !list->head)
// // 		return (NULL);
// // 	ptr = list->head;
// // 	i = 0;
// // 	tab = malloc(sizeof(char *) * (list->size + 1));
// // 	if (!tab)
// // 		return (NULL);
// // 	while (ptr)
// // 	{
// // 		printf("key = %s\n", ptr->key);  /////////
// // 		tab[i] = fill_export(ptr);
// // 		printf("tab = %s i = <%d>\n", tab[i], i); /////////
// // 		if (!tab[i])
// // 			return (free_array(tab), NULL);
// // 		i++;
// // 		ptr = ptr->next;
// // 	}
// // 	tab[i] = NULL;
// // 	return (tab);
// // }

// void	export_add(t_list *list, t_tab *arr)
// {
// 	t_node	*new_node;

// 	new_node = malloc(sizeof(t_node));
// 	if (!new_node)
// 		return ;
// 	new_node->key = ft_strdup(arr->key);
// 	if (!new_node->key)
// 		return (free(new_node));
// 	new_node->value = ft_strdup(arr->value);
// 	if (!new_node->value && arr->value)
// 		return (free(new_node->key), free(new_node));
// 	new_node->next = NULL;
// 	new_node->prev = NULL;
// 	if (list->tail != NULL)
// 	{
// 		new_node->prev = list->tail;
// 		list->tail->next = new_node;
// 	}
// 	list->tail = new_node;
// 	list->size++;
// 	if (list->head == NULL)
// 		list->head = new_node;
// }

// char	*fill_value(char *str, bool button)
// {
// 	if (ft_strchr(str, '='))
// 		return (get_value(str, button));
// 	else
// 	{
// 		if (button == false)
// 			return (ft_strdup(str));
// 		else
// 			return (NULL);
// 	}
// }

// t_node	*check_env(t_list *list, char *str)
// {
// 	t_node  *curr;

// 	curr = list->head;
// 	while (curr)
// 	{
// 		if (!ft_strncmp(str, curr->key, ft_strlen(str) + 1))
// 			return (curr);
// 		curr = curr->next;
// 	}
// 	return (NULL);
// }

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
		{
			free(curr->value);
			curr->value = ft_strdup(arr->value);
			if (!curr->value && arr->value)
				return (1);
		}
		else
		{
			curr->value = ft_strdup(arr->value);
			if (!curr->value && arr->value)
				return (1);
		}
	}
	return (0);
}

// int	word_count(char **arg)
// {
// 	int	i;

// 	i = 0;
// 	while (arg[i])
// 		i++;
// 	return (i);	
// }

// void	free_struct(t_tab ***arr)
// {
// 	int	i;

// 	i = 0;
// 	while ((*arr)[i])
// 	{
// 		if ((*arr)[i]->key)
// 			free((*arr)[i]->key);
// 		if ((*arr)[i]->value)
// 			free((*arr)[i]->value);
// 		if ((*arr)[i])
// 			free((*arr)[i]);
// 		i++;
// 	}
// 	if ((*arr))
// 		free((*arr));
// 	(*arr) = NULL;
// }

int	tab_fill(t_tab ***arr, char **arg)
{
	int	i;
	int	count;
	int	len;

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
		if (!ft_strchr(arg[i], '='))
		{
			(*arr)[i]->key = get_value(arg[i], false);
			if (!(*arr)[i]->key)
				return (free_struct(arr), 1);
			(*arr)[i]->value = NULL;
		}
		else if (ft_isalpha(arg[i][0]) || arg[i][0] == '_')
		{
			if (ft_strchr(arg[i], '+'))
			{
				len = strllen(arg[i], '=');
				if (arg[i][len - 1] == '+')
					(*arr)[i]->lever = true;
			}
			(*arr)[i]->key = get_value(arg[i], false);
			if (!(*arr)[i]->key)
				return (free_struct(arr), 1);
			(*arr)[i]->value = get_value(arg[i], true);
			if (!(*arr)[i]->value)
				return (free_struct(arr), 1);
		}
		else
		{
			print_e(1, "export", 0, NULL);
			ft_putchar_fd('`', 2);
			ft_putstr_fd(arg[i], 2);
			ft_putendl_fd("\': not a valid identifier", 2);
			(*arr)[i]->key = NULL;
			(*arr)[i]->value = get_value(arg[i], true);
			if (!(*arr)[i]->value)
				return (free_struct(arr), 1);
		}
		i++;
	}
	(*arr)[i] = NULL;
	return (0);
}

// void	print_export(t_list *list, int output)
// {
// 	int	i;

// 	sort(list->arr, list->size);
// 	i = 0;
// 	while (list->arr[i])
// 	{
// 		if (!list->arr[i]->value)
// 		{
// 			ft_putstr_fd("declare -x ", output);
// 			ft_putendl_fd(list->arr[i]->key, output);
// 			i++;
// 		}
// 		else
// 		{
// 			ft_putstr_fd("declare -x ", output);
// 			ft_putstr_fd(list->arr[i]->key, output);
// 			ft_putchar_fd('=', output);
// 			ft_putendl_fd(list->arr[i]->value, output);
// 			i++;
// 		}
// 	}
// }

// int	export_order(t_list *list, int output)
// {
// 	int	i;
// 	t_node *curr;

// 	i = 0;
// 	curr = list->head;
// 	list->arr = malloc(sizeof(t_tab *) * (list->size + 1));
// 	if (!list->arr)
// 		return (1);
// 	while (curr)
// 	{
// 		list->arr[i] = malloc(sizeof(t_tab));
// 		if (!list->arr[i])
// 			return (1);
// 		list->arr[i]->key = ft_strdup(curr->key);
// 		if (!list->arr[i]->key)
// 			return (free_struct(&list->arr), 1);
// 		list->arr[i]->value = ft_strdup(curr->value);
// 		if (!list->arr[i]->value && curr->value)
// 			return (free_struct(&list->arr), 1);
// 		i++;
// 		curr = curr->next;
// 	}
// 	list->arr[i] = NULL;
// 	print_export(list, output);
// 	free_struct(&list->arr);
// 	return (0);
// }

// int	check_key(t_tab *arr)
// {
// 	int	i;

// 	i = 0;
// 	while (arr->key[i])
// 	{
// 		if (!ft_isalnum(arr->key[i]) && !(arr->key[i] == '_'))
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

int	export(t_list *list, char **arg, int output)
{
	int		i;
	t_node	*ptr;

	output = get_output(output);
	i = 0;
	if (arg[i] == NULL)
	{
		if (export_order(list, output))
			return (free_list(list), 1);
		return (0);
	}
	else
	{
		if (tab_fill(&list->arr, arg))
			return (free_list(list), 1);
		while (list->arr[i])
		{
			if (!list->arr[i]->key)
				i++;
			else
			{
				ptr = check_env(list, list->arr[i]->key);
				if (!ptr)
				{
					if (check_key(list->arr[i]))
					{
						print_e(1, "export", 0, NULL);
						ft_putchar_fd('`', 2);
						ft_putstr_fd(arg[i], 2);
						ft_putendl_fd("\': not a valid identifier", 2);
						i++;
					}
					else
					{
						export_add(list, list->arr[i]);
						i++;
					}
				}
				else
				{
					if (change_value(ptr, list->arr[i]))
						return (free_list(list), free_struct(&list->arr), 1);
					i++;
				}
			}
		}
		free_struct(&list->arr);
		return (0);
	}
	return (1);
}
