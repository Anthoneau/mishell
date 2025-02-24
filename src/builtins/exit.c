/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:17:41 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/24 20:09:11 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exit(t_freexit to_free)
{
	free(to_free.exec->path);
	free(to_free.exec->pid);
	free_to_free(to_free.to_free);
	// free(to_free.env);
}

int	exit_builtin(char **arg, t_cmdin *cmd, t_freexit to_free)
{
	extern int	g_exit_code;
	__uint8_t	nbr;

	ft_putstr_fd("exit\n", 1);
	if (!arg[1])
		exit(g_exit_code);
	if (!ft_isnum(arg[1]))
	{
		print_error(1, "exit", 0, NULL);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_exit(to_free);
		exit(2);
	}
	if (arg[2])
		return (print_error(1, "exit", 0, "too many arguments"), 1);
	nbr = ft_atoi(arg[1]);
	free_cmd(cmd);
	rl_clear_history();
	free_exit(to_free);
	exit(nbr);
}
