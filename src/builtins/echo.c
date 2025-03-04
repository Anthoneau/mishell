/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:42:17 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/04 17:59:00 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_newline(char **arg)
{
	int	i;

	i = 0;
	if (arg[1][i] == '-')
	{
		i++;
		while (arg[1][i])
		{
			if (arg[1][i] != 'n')
				return (1);
			i++;
		}
		return (0);
	}
	return (1);
}

int	echo(char **arg)
{
	int	i;
	int	newline;

	if (!arg[1])
		return (ft_putchar_fd('\n', 1), 0);
	newline = check_newline(arg);
	if (newline == 0)
		i = 2;
	else
		i = 1;
	while (arg[i])
	{
		ft_putstr_fd(arg[i], 1);
		i++;
		if (arg[i])
			ft_putchar_fd(' ', 1);
	}
	if (newline == 1)
		ft_putchar_fd('\n', 1);
	return (0);
}
