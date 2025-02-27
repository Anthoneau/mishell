/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:54:08 by qalpesse          #+#    #+#             */
/*   Updated: 2024/06/17 13:53:22 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strlen_bn(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	return (i);
}

static char	*ft_freestr(char *str)
{
	if (str != NULL)
		free(str);
	return (NULL);
}

char	*ft_strjoin_mod(char *s1, char *s2)
{
	char	*ptr;
	char	*tmp;
	char	*sub_s1;

	sub_s1 = s1;
	ptr = malloc(ft_strlen_bn(s1) + ft_strlen_bn(s2) + 1);
	if (!ptr)
		return (ft_freestr(s1));
	tmp = ptr;
	if (s1)
	{
		while (*s1 && *s1 != '\n')
			*tmp++ = *s1++;
	}
	while (*s2 && *s2 != '\n')
		*tmp++ = *s2++;
	*tmp = '\0';
	ft_freestr(sub_s1);
	return (ptr);
}

int	ft_findchar(char c, char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}
