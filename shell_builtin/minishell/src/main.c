/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:52:51 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/25 13:20:41 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_code;

int	get_name(t_data *d)
{
	d->name = minishell_name(d->env);
	if (!d->name)
		return (print_error(1, "malloc", 1, ""), 0);
	return (1);
}

int	do_minishell(t_data d, int eof)
{
	while (eof == 0)
	{
		set_signal_action(0);
		if (!get_name(&d))
			return (1);
		d.inpt = readline(d.name);
		if (d.inpt && *d.inpt && !ft_isspace(d.inpt))
		{
			d.token = lexer(d.inpt, d.env);
			if (d.token && check_token(&d.token, &d.inpt, d.env))
			{
				d.ast = create_ast(&d.token, NULL, 0, &d.error);
				if (d.ast)
					exec(d.ast, d.env, get_to_free(&d.name, &d.token, &d.ast));
				free_ast(d.ast);
			}
			add_history(d.inpt);
			free_token(&d.token);
		}
		else if (!d.inpt)
			eof = 1;
		ft_free(d.inpt);
		free(d.name);
	}
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
	data.env = env;
	g_exit_code = 0;
	res = do_minishell(data, 0);
	if (g_exit_code != 0)
		res = g_exit_code;
	rl_clear_history();
	ft_putstr_fd("exit\n", 1);
	return (res);
}
