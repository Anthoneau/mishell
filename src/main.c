/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:52:51 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/27 20:01:01 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code;

int	get_name(t_data *d)
{
	char	**env;

	env = get_env(d->env);
	d->name = minishell_name(env);
	free_array(env);
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
		free(d.name);
		if (d.inpt && *d.inpt && !ft_isspace(d.inpt))
		{
			d.token = lexer(d.inpt, get_env(d.env));
			if (d.token && check_token(&d.token, &d.inpt, get_env(d.env)))
			{
				d.ast = create_ast(&d.token, NULL, 0, &d.error);
				free_token(&d.token);
				if (d.ast)
					exec(d.ast, &d.env);
			}
			add_history(d.inpt);
		}
		else if (!d.inpt)
			eof = 1;
		ft_free(d.inpt);
	}
	free_list(d.env);
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
	// data.env = env;
	make_list(data.env, env);
	g_exit_code = 0;
	res = do_minishell(data, 0);
	if (g_exit_code != 0)
		res = g_exit_code;
	rl_clear_history();
	ft_putstr_fd("exit\n", 1);
	return (res);
}
