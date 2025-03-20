/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:54:05 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/20 14:18:34 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_swap_char(char **s1, char **s2)
{
	char	*temp;

	temp = *s2;
	*s2 = *s1;
	*s1 = temp;
}

void	remove_empty(t_cmdin *cmd)
{
	int	i;

	i = 0;
	if (!cmd->cmd->arg[i + 1])
		return ;
	while (cmd->cmd->arg[i])
	{
		if (!cmd->cmd->arg[i][0])
		{
			free(cmd->cmd->arg[i]);
			cmd->cmd->arg[i] = NULL;
		}
		i++;
	}
}

void	argument_checker(t_cmdin *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd->cmd->arg[i])
	{
		if (!cmd->cmd->arg[i][0])
		{
			j = i + 1;
			while (cmd->cmd->arg[j])
			{
				if (cmd->cmd->arg[j][0])
				{
					ft_swap_char(&cmd->cmd->arg[i], &cmd->cmd->arg[j]);
					break ;
				}
				j++;
			}
		}
		i++;
	}
	remove_empty(cmd);
}
