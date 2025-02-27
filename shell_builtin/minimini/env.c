/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 15:37:26 by dell              #+#    #+#             */
/*   Updated: 2025/02/24 20:09:09 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void	ft_free(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*ft_strldup(const char *s1, int len)
{
	//printf("<%s> len=<%d>\n", s1, len);
	int		i;
	char	*str;

	i = 0;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i != len)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	//printf("final str<%s>\n", str);
	return (str);
}

int	strllen(char *s, char c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while(s[i] && s[i] != c)
		i++;
	return (i);
}
char	*get_value(char *str, bool button)
{
	//printf("***get_value\n");
	int	len;
	char	*s;

	len = strllen(str, '=');
	if (button == false)
	{
		s = ft_strldup(str, len);
		//printf("<%s>\n", s);
	}
	else if (button == true)
		s = ft_strldup(str + (len + 1), ft_strlen(str + (len + 1)));
	return (s);
}

void	get_list(t_list *list, char *str)
{
	//printf("***get list\n");
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
	return ;
	new_node->key = get_value(str, false);
	//printf("<%s>\n", new_node->key);
	new_node->value = get_value(str, true);
	new_node->next = NULL;
	new_node->prev = NULL;
	if (list->tail != NULL)
	{
		new_node->prev = list->tail;
		list->tail->next = new_node;
	}
	list->tail = new_node;
	list->size++;
	if (list->head == NULL)
		list->head = new_node;
}

void	init_list(t_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

// void	get_export_order(t_list *list)
// {
// 	char	**tmp_tab;
// 	t_node	*ptr;
// 	int		i;
	
// 	i = 0;
// 	ptr = list->head;
// 	tmp_tab = malloc(sizeof(char *) * (list->size));
// 	if (!tmp_tab)
// 		return ;
// 	while (i < list->size)
// 	{
// 		tmp_tab[i] = ft_strdup(ptr->key);
// 		i++;
// 		ptr = ptr->next;
// 	}
// 	tmp_tab[i] = NULL;
// 	sort(tmp_tab, list->size);
// 	int j = 0; //////////////
// 	while (tmp_tab[j])
// 	{
// 		printf("%s\n", tmp_tab[j]);
// 		j++;
// 	}
// }

void	make_list(t_list *list, char **env)
{
	printf("***make list\n");
	int	i;

	init_list(list);
	i = 0;
	while (env[i])
	{
		get_list(list, env[i]);
		i++;
	}
	// get_export_order(list);
}


/*  


	la partie en charge de chercher env  


*/
char	*fill_tab(t_node *list)
{
	char	*str;

	str = ft_strjoin(list->key, "=");
	str = ft_strjoin(str, list->value);
	return (str);
}

char	**get_env(t_list *list)
{
	t_node	*ptr;
	char	**tab;
	int		i;

	ptr = list->head;
	i = 0;
	tab = malloc(sizeof(char *) * (list->size + 1));
	if (!tab)
		return (NULL);
	while (ptr != NULL)
	{
		tab[i] = fill_tab(ptr);
		i++;
		ptr = ptr->next;
	}
	tab[i] = NULL;
	return (tab);
}
void	export_order(t_list *list)
{
	char	**export;

	export = get_env(list);
	sort(export, list->size);
	///////////////
	int i = 0;
	while (export[i])
		printf("declare -x %s\n", export[i++]);
	ft_free(export);
}

int main(int ac, char **av, char **envp)
{
	//printf("main\n");
	(void)av;
	(void)ac;
	t_list	list;
	// char	**tab;

	// init_list(&list);
	make_list(&list, envp);
	// tab = get_env(&list);
	// int i = 0;
	// while (tab[i])
	// {
	// 	printf("%s\n", tab[i]);
	// 	i++;
	// }
	// ft_free(tab);
	// unset(&list, "QT_IM_MODULE");
	export_order(&list);
	// tab = get_env(&list);
	// i = 0;
	// while (tab[i])
	// {
	// 	printf("%s\n", tab[i]);
	// 	i++;
	// }
	// t_node	*ptr;
	// ptr = list.head;
	// while (ptr != NULL)
	// {
	// 	printf("%s\n", ptr->key);
	// 	ptr = ptr->next;
	// }
	// printf("<%d>\n", list.size);
	return (0);
}