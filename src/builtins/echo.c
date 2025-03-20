/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:42:17 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/19 18:17:09 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_newline(char **arg)
{
	int	i;
	int	j;
	int	begin;

	i = 1;
	begin = 1;
	while (arg[i])
	{
		j = 0;
		if (arg[1][0] == '-' && arg[i][j] == '-')
		{
			j++;
			while (arg[i][j])
			{
				if (arg[i][j] != 'n')
					return (begin);
				j++;
			}
			begin++;
		}
		i++;
	}
	return (begin);
}

int	echo(t_cmd *cmd, int output)
{
	int			i;
	int			newline;

	if (output == -2)
		return (1);
	output = get_output(output);
	if (!cmd->arg[1])
		return (ft_putchar_fd('\n', output), 0);
	newline = check_newline(cmd->arg);
	i = newline;
	while (cmd->arg[i])
	{
		ft_putstr_fd(cmd->arg[i], output);
		i++;
		if (cmd->arg[i])
			ft_putchar_fd(' ', output);
	}
	if (newline == 1)
		ft_putchar_fd('\n', output);
	return (0);
}
