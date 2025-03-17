/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:42:17 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/17 16:14:49 by agoldber         ###   ########.fr       */
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

int	echo(t_cmd *cmd, int output)
{
	int			i;
	int			newline;

	output = get_output(output);
	if (!cmd->arg[1])
		return (ft_putchar_fd('\n', output), 0);
	newline = check_newline(cmd->arg);
	if (newline == 0)
		i = 2;
	else
		i = 1;
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
