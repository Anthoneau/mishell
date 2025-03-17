/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/14 20:02:21 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	strllen(char *s, char c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	*ft_strldup(const char *s1, int len)
{
	int		i;
	char	*str;

	if (!s1)
		return (NULL);
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
	return (str);
}

void	free_tab_arr(t_tab **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		if (arr[i]->key)
			free(arr[i]->key);
		if (arr[i]->value)
			free(arr[i]->value);
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_list(t_list *env)
{
	t_node	*ptr;
	t_node	*next;

	ptr = env->head;
	if (ptr && ptr->next)
		next = ptr->next;
	while (ptr)
	{
		if (ptr->key)
			free(ptr->key);
		if (ptr->value)
			free(ptr->value);
		free(ptr);
		ptr = next;
		if (ptr)
			next = ptr->next;
	}
	if (env->pwd)
		free(env->pwd);
	if (env->oldpd)
		free(env->oldpd);
	free_tab_arr(env->arr);
	free(env);
}

t_node	*get_node(t_list *list, char *s)
{
	t_node	*ptr;
	int		cmp;

	ptr = list->head;
	while (ptr != NULL)
	{
		cmp = ft_strncmp(s, ptr->key, ft_strlen(s) + 1);
		if (cmp == 0)
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}
