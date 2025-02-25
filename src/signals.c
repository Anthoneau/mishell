/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:36:32 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/25 13:21:42 by agoldber         ###   ########.fr       */
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

void	ignore_signal(int sig)
{
	(void)sig;
}

void	handle_heredoc(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putchar_fd('\n', 1);
	exit(130);
}

void	signal_g_exit_code(int status, t_cmdin cmd)
{
	int			sig;
	extern int	g_exit_code;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		g_exit_code = 130;
	}
	else if (sig == SIGQUIT)
	{
		if (cmd.num_of_cmds == 1)
			ft_putstr_fd("Quit (core dumped)\n", 1);
		g_exit_code = 131;
	}
}

void	set_signal_action(int mode)
{
	struct sigaction	act_int;
	struct sigaction	act_quit;

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
	sigaction(SIGINT, &act_int, NULL);
	sigaction(SIGQUIT, &act_quit, NULL);
}
