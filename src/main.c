/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:52:51 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/13 16:55:57 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code;

int	get_name(t_data *d)
{
	set_signal_action(0);
	d->env->env_c = get_env(d->env);
	if (!d->env->env_c)
		return (print_e(1, "malloc", 1, ""), 0);
	d->name = minishell_name(d);
	if (!d->name)
		return (free_array(d->env->env_c), print_e(1, "malloc", 1, ""), 0);
	return (1);
}

int	do_minishell(t_data *d, int eof)
{
	while (eof == 0)
	{
		if (!get_name(d))
			return (1);
		d->inpt = readline(d->name);
		free(d->name);
		if (d->inpt && *d->inpt && !ft_isspace(d->inpt))
		{
			d->token = lexer(d->inpt, d->env->env_c);
			if (d->token && check_token(&d->token, &d->inpt, d->env->env_c))
			{
				d->ast = create_ast(&d->token, NULL, 0, &d->error);
				free_token(&d->token);
				if (d->ast)
					exec(d->ast, &d->env);
			}
			add_history(d->inpt);
		}
		else if (!d->inpt)
			eof = 1;
		ft_free(d->inpt);
	}
	free_list(d->env);
	system("leaks minishell");
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	int		res;

	(void)ac;
	(void)av;
	data.error = 0;
	data.name = NULL;
	data.inpt = NULL;
	data.token = NULL;
	data.ast = NULL;
	data.env = make_list(env);
	if (!data.env)
		return (print_e(1, "malloc", 1, NULL), 1);
	g_exit_code = 0;
	res = do_minishell(&data, 0);
	if (g_exit_code != 0)
		res = g_exit_code;
	rl_clear_history();
	ft_putstr_fd("exit\n", 1);
	return (res);
}
