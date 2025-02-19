/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:36:32 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/19 18:00:53 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	normal_signal(int signal_i)
{
	if (signal_i == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		signal(signal_i, SIG_IGN);
		// ft_putchar_fd('\0', 1);
		// printf("SIGQUIT : %d\n", signal);
// 		signal(SIGQUIT, SIG_IGN);
}

void	heredoc_signal(int sig)
{
	// if (signal_i == SIGINT)
	// {
	(void)sig;
	rl_replace_line("", 1);
	ft_putchar_fd('\0', 1);
	// rl_on_new_line();
	exit(130);
	// }
	// else if (signal_i == SIGQUIT)
	// 	// printf("SIGQUIT : %d\n", signal);
	// 	signal(SIGQUIT, SIG_IGN);
}

void	nothing_signal(int sig)
{
	signal(sig, SIG_IGN);
	// printf("ye %d\n", sig);
	// if (sig == SIGINT)
	// {
	// 	ft_putchar_fd('\n', 1);
	// 	rl_replace_line("", 1);
	// 	rl_on_new_line();
	// 	rl_redisplay();
	// }
	// else if (sig == SIGQUIT)
	// 	ft_putchar_fd('\0', 1);
}

void	set_signal_action(int mode)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	if (mode == 0)
		act.sa_handler = &normal_signal;
	else if (mode == 1)
		act.sa_handler = &heredoc_signal;
	else if (mode == 2)
		act.sa_handler = &nothing_signal;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}
