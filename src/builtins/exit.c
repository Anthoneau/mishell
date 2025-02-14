/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:17:41 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/14 15:14:39 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_builtin(char **arg)
{
	extern int	exit_code;
	__uint8_t		nbr;

	if (!arg[1])
		exit(exit_code);
	if (!ft_isnum(arg[1]))
	{
		print_error(1, "exit", NULL);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return ;
	}
	if (arg[2])
		return (print_error(1, "exit", "too many arguments"));
	nbr = ft_atoi(arg[1]);
	exit(nbr);
}
