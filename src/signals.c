/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:36:32 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/20 14:15:48 by agoldber         ###   ########.fr       */
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

void	handle_heredoc(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putchar_fd('\n', 1);
	exit(130);
}

void	ignore_signal(int sig)
{
	(void)sig;
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
		// printf("mode 0 -- c pas \\\n");
	}
	else if (mode == 1)
	{
		act_int.sa_handler = &handle_heredoc;
		act_quit.sa_handler = SIG_IGN;
		// printf("mode 1 heredoc\n");
	}
	else if (mode == 2)
	{
		act_int.sa_handler = &ignore_signal;
		act_quit.sa_handler = &ignore_signal;
		// printf("mode 2 -- ignore\n");
	}
	else if (mode == 3)
	{
		act_int.sa_handler = &handle_sigint;
		act_quit.sa_handler = &handle_execsig;
		// printf("mode 3 -- c et \\ \n");
	}
	sigaction(SIGINT, &act_int, NULL);
	sigaction(SIGQUIT, &act_quit, NULL);
}


// void	normal_signal(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		ft_putchar_fd('\n', 1);
// 		rl_replace_line("", 1);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}

// 		// ft_putchar_fd('\0', 1);
// 		// printf("SIGQUIT : %d\n", signal);
// // 		signal(SIGQUIT, SIG_IGN);
// }

// void	heredoc_signal(int sig)
// {
// 	// if (signal_i == SIGINT)
// 	// {
// 	(void)sig;
// 	rl_replace_line("", 1);
// 	ft_putchar_fd('\0', 1);
// 	// rl_on_new_line();
// 	exit(130);
// 	// }
// 	// else if (signal_i == SIGQUIT)
// 	// 	// printf("SIGQUIT : %d\n", signal);
// 	// 	signal(SIGQUIT, SIG_IGN);
// }

// void	nothing_signal(int sig)
// {
// 	signal(sig, SIG_IGN);
// 	// printf("ye %d\n", sig);
// 	// if (sig == SIGINT)
// 	// {
// 	// 	ft_putchar_fd('\n', 1);
// 	// 	rl_replace_line("", 1);
// 	// 	rl_on_new_line();
// 	// 	rl_redisplay();
// 	// }
// 	// else if (sig == SIGQUIT)
// 	// 	ft_putchar_fd('\0', 1);
// }

// void	set_signal_action(int mode)
// {
// 	struct sigaction	act;

// 	ft_bzero(&act, sizeof(act));
// 	if (mode == 0)
// 		act.sa_handler = &normal_signal;
// 	else if (mode == 1)
// 		act.sa_handler = &heredoc_signal;
// 	else if (mode == 2)
// 		act.sa_handler = &nothing_signal;
// 	sigaction(SIGINT, &act, NULL);
// 	sigaction(SIGQUIT, &act, NULL);
// }
