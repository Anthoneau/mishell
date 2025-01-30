/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:52:51 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/30 13:47:57 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	*name_minishell(char *pwd, char *home)
// {
// 	char	*name;
// 	char	*temp;
// 	int		i;
// 	int		count;

// 	i = 0;
// 	count = 0;
// 	while (pwd[i] == home[i])
// 		i++;
// 	temp = ft_strjoin("~", pwd + i);
// 	i = 0;
// 	while (temp[i])
// 	{
// 		if (temp[i] == '/')
// 			count++;
// 		i++;
// 	}
// 	if (count >= 3)
// 	{
// 		int count2 = 0;
// 		while (count2 != 3 || i != 0)
// 		{
// 			if (temp[i] == '/')
// 				count2++;
// 			if (count2 == 3)
// 				break ;
// 			i--;
// 		}
// 		i++;
// 	}
// 	else
// 		i = 0;
// 	name = ft_strjoin("\x1b[0;32m", temp + i);
// 	free(temp);
// 	temp = ft_strjoin(name, "\x1b[0m\x1b[0;34m » \x1b[0m");
// 	free(name);
// 	return (temp);
// }

// static char	*get_home(char **env)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (env[i])
// 	{
// 		if (ft_strnstr(env[i], "HOME", 5))
// 			break ;
// 		i++;
// 	}
// 	while (env[i][j] != '=')
// 		j++;
// 	j++;
// 	return (env[i] + j);
// }

int	main(int ac, char **av, char **env)
{
	char		*inpt;
	// char		*name;
	// char		*home;
	t_token		*token;
	t_ast		*ast;
	int			error;

	(void)ac;
	(void)av;
	(void)env;
	// home = get_home(env);
	token = NULL;
	error = 0;
	while (1)
	{
		// name = name_minishell(getcwd(NULL, 0), home);
		inpt = readline("Mishell > ");
		// free(name);
		if (inpt && *inpt && !ft_isspace(inpt))
		{
			printf("on passe dans le lexer\n");
			token = lexer(inpt);
			if (token && check_token(&token, &inpt, env) && expander(&token, env))
			{
				display_token(&token);
				printf("\n");
				//sleep(1);
				printf("on passe dans le parser\n");
				ast = create_ast(&token, NULL, 0, &error);
				if (ast)
				{
					//printf("create_ast fonctionne !\nretour au main\n");
					//printf("on dessine l'ast total\n");
					//sleep(1);
					draw_ast(ast, 0);
					printf("\n");
					//redir ?
					printf("exec retourne %d\n", exec(ast, env, count_pipes(token)));
					free_ast(ast);
				}
				else
					printf("ast est NULL\n");
			}
			// printf("main : inpt = %s\n", inpt);
			add_history(inpt);
			if (token)
				free_token(&token);
		}
		if (inpt)
			free(inpt);
	}
	return (0);
}
