/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:52:51 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 12:27:02 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_code;

int	do_minishell(t_data d)
{
	while (1)
	{
		d.name = minishell_name(d.env);
		if (!d.name)
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
		ft_free(d.inpt);
		free(d.name);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_data data;

	(void)ac;
	(void)av;
	data.error = 0;
	data.name = NULL;
	data.inpt = NULL;
	data.token = NULL;
	data.ast = NULL;
	data.env = env;
	exit_code = 0;
	return (do_minishell(data));
}
