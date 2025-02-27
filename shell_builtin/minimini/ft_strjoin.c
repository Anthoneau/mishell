/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dell <dell@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 11:05:50 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/12 00:06:21 by dell             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	total;
	size_t	j;

	total = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = malloc(sizeof(char) * total);
	if (!joined)
		return (NULL);
	j = 0;
	while (s1 && *s1)
	{
		joined[j] = *s1;
		s1++;
		j++;
	}
	while (s2 && *s2)
	{
		joined[j] = *s2;
		s2++;
		j++;
	}
	joined[j] = '\0';
	return (joined);
}
