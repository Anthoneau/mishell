/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:23:53 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/26 11:49:54 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *content)
{
	if ((ft_strncmp(content, "echo", 5) == 0) || !ft_strncmp(content, "cd", 3)
		|| !ft_strncmp(content, "pwd", 4) || !ft_strncmp(content, "export", 7)
		|| !ft_strncmp(content, "unset", 6) || !ft_strncmp(content, "env", 4)
		|| !ft_strncmp(content, "exit", 5))
		return (1);
	return (0);
}

int	do_builtins(char **arg)
{
	if (!ft_strncmp(arg[0], "echo", 5))
		return (echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 3))
		return (printf("cd\n"));
	else if (!ft_strncmp(arg[0], "pwd", 4))
		return (printf("pwd\n"));
	else if (!ft_strncmp(arg[0], "export", 7))
		return (printf("export\n"));
	else if (!ft_strncmp(arg[0], "unset", 6))
		return (printf("unset\n"));
	else if (!ft_strncmp(arg[0], "env", 4))
		return (printf("env\n"));
	return (0);
}

int	exec_builtins(t_exec *exc, t_cmdin **cmd, char **env)
{
	extern int	g_exit_code;

	(void)env;
	exc->builtin = 1;
	if (!ft_strncmp((*cmd)->cmd[0].arg[0], "exit", 5))
	{
		ft_free(exc->pid);
		ft_free(exc->path);
		exit_builtin((*cmd)->cmd[0].arg, cmd);
		g_exit_code = 1;
		return (0);
	}
	g_exit_code = do_builtins((*cmd)->cmd[0].arg);
	return (1);
}
