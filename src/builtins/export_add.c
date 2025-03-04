// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   export_add.c                                       :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/04 13:59:34 by mel-bout          #+#    #+#             */
// /*   Updated: 2025/03/04 16:36:10 by mel-bout         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// char	*fill_tab(t_node *list)
// {
// 	char	*key;
// 	char	*str;

// 	if (!list || !list->value || !list->key)
// 		return (NULL);
// 	key = ft_strjoin(list->key, "=");
// 	if (!key)
// 		return (NULL);
// 	str = ft_strjoin(key, list->value);
// 	free(key);
// 	if (!str)
// 		return (NULL);
// 	return (str);
// }

// int list_len(t_list2 **list)
// {
//     int i;
//     t_list2 *ptr;

//     i = 0;
//     ptr = (*list);
//     printf("%p\n", ptr);
//     while (ptr != NULL)
//     {
//         i++;
//         ptr = ptr->next;   
//     }
//     return (i);
// }
// char	**get_env_add(t_list2 *list)
// {
// 	t_node	*ptr;
// 	char	**tab;
// 	int		i;

// 	if (!list)
// 		return (NULL);
// 	ptr = list;
// 	i = 0;
// 	tab = malloc(sizeof(char *) * (list_len(list) + 1));
// 	if (!tab)
// 		return (NULL);
// 	while (ptr)
// 	{
// 		tab[i] = fill_tab(ptr);
// 		if (!tab[i])
// 			return (free_array(tab), NULL);
// 		i++;
// 		ptr = ptr->next;
// 	}
// 	tab[i] = NULL;
// 	return (tab);
// }

// void	export_order_add(t_list2 *list)
// {
// 	char	**export;
// 	int     i;

// 	export = get_env_add(list);
// 	sort(export, list->size);
// 	i = 0;
// 	while (export[i])
// 		printf("declare -x %s\"\n", export[i++]);
// 	free_array(export);
// }

