/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/04 17:36:46 by agoldber         ###   ########.fr       */
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

void	free_list(t_list *env)
{
	t_node	*ptr;
	t_node	*next;

	ptr = env->head;
	next = ptr->next;
	while (ptr != NULL)
	{
		free(ptr->key);
		free(ptr->value);
		free(ptr);
		ptr = next;
		if (ptr != NULL)
			next = ptr->next;
	}
}
