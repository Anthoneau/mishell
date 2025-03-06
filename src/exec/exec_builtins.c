/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:23:53 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/06 15:46:26 by mel-bout         ###   ########.fr       */
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

int	do_builtins(char **arg, t_list **env)
{
	if (!ft_strncmp(arg[0], "echo", 5))
		return (echo(arg));
	else if (!ft_strncmp(arg[0], "cd", 3))
		return (cd(arg, env));
	else if (!ft_strncmp(arg[0], "pwd", 4))
		return (pwd(*env));
	else if (!ft_strncmp(arg[0], "export", 7))
		return (export(*env, arg + 1));
	else if (!ft_strncmp(arg[0], "unset", 6))
		return (unset(*env, arg + 1));
	else if (!ft_strncmp(arg[0], "env", 4))
		return (call_env(*env));
	return (0);
}

int	exec_builtins(t_exec *exc, t_cmdin **cmd, t_list **env)
{
	extern int	g_exit_code;

	exc->builtin = 1;
	if (!ft_strncmp((*cmd)->cmd[0].arg[0], "exit", 5))
	{
		ft_free(exc->pid);
		ft_free(exc->path);
		free_list(*env);
		exit_builtin((*cmd)->cmd[0].arg, cmd);
		g_exit_code = 1;
		return (0);
	}
	g_exit_code = do_builtins((*cmd)->cmd[0].arg, env);
	return (1);
}
