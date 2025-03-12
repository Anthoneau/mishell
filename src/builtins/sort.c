/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:08:02 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/12 19:34:42 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap(t_tab **s1, t_tab **s2)
{
	t_tab	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

void	sort(t_tab **arr, int size)
{
	int	cmp;
	int	i;
	int	j;
	int	len;

	i = size - 1;
	while (i > 0)
	{
		j = 0;
		while (j < i)
		{
			len = ft_strlen(arr[j]->key) + 1;
			cmp = ft_strncmp(arr[j]->key, arr[j + 1]->key, len);
			if (cmp > 0)
				swap(&arr[j], &arr[j + 1]);
			j++;
		}
		i--;
	}
}
