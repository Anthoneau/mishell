/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:02:27 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/10 15:59:04 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*fill_export(t_node *list)
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

char	**export_arr(t_list *list)
{
	t_node	*ptr;
	char	**tab;
	int		i;

	if (!list || !list->head)
		return (NULL);
	ptr = list->head;
	i = 0;
	tab = malloc(sizeof(char *) * (list->size + 1));
	if (!tab)
		return (NULL);
	while (ptr)
	{
		printf("key = %s\n", ptr->key);  /////////
		tab[i] = fill_export(ptr);
		printf("tab = %s i = <%d>\n", tab[i], i); /////////
		if (!tab[i])
			return (free_array(tab), NULL);
		i++;
		ptr = ptr->next;
	}
	tab[i] = NULL;
	return (tab);
}

// void	export_order(t_list *list)
// {
// 	char	**export;
// 	int     i;

// 	export = export_arr(list);
// 	sort(export, list->size);
// 	i = 0;
// 	while (export[i])
// 		printf("declare -x %s\n", export[i++]);
// 	free_array(export);
// }

char	*fill_value(char *str, bool button)
{
	if (ft_strchr(str, '='))
		return (get_value(str, button));
	else
	{
		if (button == false)
			return (ft_strdup(str));
		else
			return (NULL);
	}
}

t_node	*check_env(t_list *list, char *str)
{
	char	*s;
	int		len;
	t_node  *curr;

	len = strllen(str, '=');
	s = ft_strldup(str, len);
	curr = list->head;
	while (curr)
	{
		if (!ft_strncmp(s, curr->key, ft_strlen(s) + 1))
		{
			free(s);
			return (curr);
		}
		curr = curr->next;
	}
	free(s);
	return (NULL);
}

void	change_value(t_node *curr, char *str)
{
	int		len;

	len = strllen(str, '=');
	if (curr->value)
	{
		free(curr->value);
		curr->value = ft_strldup(str + (len + 1), ft_strlen(str + (len + 1)));	
	}
}
int	word_count(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);	
}

void	tab_fill(t_tab **arr, char **arg)
{
	int	i;
	int	j;
	int	count;

	count = word_count(arg);
	arr = malloc(sizeof(t_tab *) * (count + 1));
	if (arr)
		return ;
	while (arg[i])
	{
		arr[i] = malloc(sizeof(t_tab)); // protection
		arr[i]->key = ft_strdup(curr->key); // protection
		arr[i]->value = ft_strdup(curr->value); // protection
		i++;
		curr = curr->next;
	}
	arr[i] = NULL;
}
void	export_order(t_list *list)
{
	int	i;
	t_node *curr;

	curr = list->head;
	list->arr = malloc(sizeof(t_tab *) * (list->size + 1));
	if (!list->arr)
		return ;
	while (curr)
	{
		list->arr[i] = malloc(sizeof(t_tab)); // protection
		list->arr[i]->key = ft_strdup(curr->key); // protection
		list->arr[i]->value = ft_strdup(curr->value); // protection
		i++;
		curr = curr->next;
	}
	list->arr[i] = NULL;
	sort(list->arr, list->size);
	i = 0;
	while (list->arr[i])
	{
		printf("declare -x %s=\"%s\"\n", list->arr[i]->key, list->arr[i]->value);
		i++;
	}  // il faut free
}

int	export(t_list *list, char **arg)
{
	int		i;
	// t_node	*curr;
	t_node	*ptr;

	i = 0;
	if (arg[i] == NULL)
	{
		// export_order(list);
		export_order(list);
		return (0);
	}
	else
	{

		while(arg[i])
		{
			ptr = check_env(list, arg[i]);
			if (!ptr)
			{
				get_list(list, arg[i]);
				// if (!list->tail)
				// return (free_list(list), 1);
			}
			else
				change_value(ptr, arg[i]);
			i++;
		}
		////////////////////////
		// curr = list->head;
		// while (curr != NULL)
		// {
		// 	printf("%s\n", curr->key);
		// 	curr = curr->next;
		// }
		// // printf("%p key = %s and %d\n", list->add_key, list->add_key->key, list_len(&list->add_key));
		// printf("list size = %d\n", list->size);
		// printf("fin add_node\n");
		////////////////////////
		return (0);
	}
	return (1);
}
