/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 13:29:05 by mel-bout          #+#    #+#             */
/*   Updated: 2024/05/05 12:36:58 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void	swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

void	sort(char **arr, int size)
{
	int	cmp;
	int	i;
	int	j;

	i = size - 1;
	while (i > 0)
	{
		j = 0;
		while (j < i)
		{
			cmp = ft_strncmp(arr[j], arr[j + 1], ft_strlen(arr[j]) + 1);
			if (cmp > 0)
				swap(&arr[j], &arr[j + 1]);
			j++;
		}
		i--;
	}
}
// int main(int ac, char **av)
// {
// 	char	**str;
// 	int		i;

// 	i = 0;
// 	str = malloc(sizeof(char *) * ac);
// 	if (!str)
// 		return (0);
// 	// printf("%d\n", ac);
// 	while (av[i])
// 	{
// 		str[i] = ft_strdup(av[i]);
// 		i++;
// 	}
// 	str[i] = NULL;
// 	sort(str, ac);
// 	// printf("%d\n", i);
// 	i = 0;
// 	while (str[i])
// 		printf("%s\n", str[i++]);
// 	ft_free(str);
// 	return (0);
// }
