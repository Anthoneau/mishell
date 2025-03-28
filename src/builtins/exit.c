/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:17:41 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/26 16:04:14 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_and_exit(t_cmdin **cmd, int n)
{
	free_cmd(*cmd);
	rl_clear_history();
	exit(n);
}

int	exit_builtin(char **arg, t_cmdin **cmd, int output, bool child)
{
	extern int	g_exit_code;
	__uint8_t	nbr;

	output = get_output(output);
	if (!child)
		ft_putstr_fd("exit\n", output);
	if (!arg[1])
		free_and_exit(cmd, g_exit_code);
	if (!ft_isnum(arg[1]))
	{
		print_e(1, "exit", 0, NULL);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_and_exit(cmd, 2);
	}
	if (arg[2])
		return (print_e(1, "exit", 0, "too many arguments"), 1);
	nbr = ft_atoi(arg[1]);
	free_and_exit(cmd, nbr);
	return (0);
}
