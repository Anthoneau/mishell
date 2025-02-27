/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 13:29:05 by mel-bout          #+#    #+#             */
/*   Updated: 2025/02/20 20:37:26 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] - s2[i] != 0 || s1[i] == 0)
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
/*#include <string.h>
int main()
{
	char s1[10] = "abcd\0fewf";
	char s2[10] = "abcd\0fer";
	int a = 6;

	printf("%s %d\n", s1, strncmp(s1, s2, a));
	printf("%d\n", ft_strncmp(s1, s2, a));
}*/
