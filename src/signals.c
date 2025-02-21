/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:36:32 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/21 15:33:12 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	rl_on_new_line();
	ft_putchar_fd('\n', 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_execsig(int sig)
{
	(void)sig;
	rl_on_new_line();
	// ft_putchar_fd('', 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ignore_signal(int sig)
{
	(void)sig;
}

void	handle_exit(int sig)
{
	(void)sig;
	rl_on_new_line();
	ft_putchar_fd('\n', 1);
	rl_replace_line("", 0);
	rl_redisplay();
	exit(130);
}

void	handle_heredoc(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putchar_fd('\n', 1);
	exit(130);
}


void	set_signal_action(int mode)
{
	struct sigaction act_int;
	struct sigaction act_quit;

	sigemptyset(&act_int.sa_mask);
	sigemptyset(&act_quit.sa_mask);
	act_int.sa_flags = SA_RESTART;
	act_quit.sa_flags = SA_RESTART;
	if (mode == 0)
	{
		act_int.sa_handler = &handle_sigint;
		act_quit.sa_handler = SIG_IGN;
	}
	else if (mode == 1)
	{
		act_int.sa_handler = &handle_heredoc;
		act_quit.sa_handler = SIG_IGN;
	}
	else if (mode == 2)
	{
		act_int.sa_handler = &ignore_signal;
		act_quit.sa_handler = &ignore_signal;
	}
	else if (mode == 3)
	{
		act_int.sa_handler = &handle_heredoc;
		act_quit.sa_handler = &handle_heredoc;
	}
	sigaction(SIGINT, &act_int, NULL);
	sigaction(SIGQUIT, &act_quit, NULL);
}
