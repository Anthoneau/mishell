/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:17:41 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/26 11:32:44 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_builtin(char **arg, t_cmdin **cmd)
{
	extern int	g_exit_code;
	__uint8_t	nbr;

	ft_putstr_fd("exit\n", 1);
	if (!arg[1])
	{
		free_cmd(*cmd);
		rl_clear_history();
		exit(g_exit_code);
	}
	if (!ft_isnum(arg[1]) && !arg[2])
	{
		print_error(1, "exit", 0, NULL);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_cmd(*cmd);
		rl_clear_history();
		exit(2);
	}
	if (arg[2])
		return (print_error(1, "exit", 0, "too many arguments"), 1);
	nbr = ft_atoi(arg[1]);
	free_cmd(*cmd);
	rl_clear_history();
	exit(nbr);
}
