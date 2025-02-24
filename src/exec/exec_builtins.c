/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:23:53 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/24 17:50:12 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_code;

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
		return (printf("echo\n"));
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
	// else if (!ft_strncmp(arg[0], "exit", 5))
	// 	return (exit_builtin(arg, cmd));
	return (0);
}

t_freexit	get_freexit(t_exec *exec, t_free to_free, char **env)
{
	t_freexit	free_exit;

	free_exit.exec = exec;
	free_exit.to_free = to_free;
	free_exit.env = env;
	return (free_exit);
}

void	exec_builtins(t_exec *exec, t_cmd_info *cmd, t_free to_free, char **env)
{
	exec->builtin = 1;
	if (!ft_strncmp(cmd->cmd[0].arg[0], "exit", 5))
	{
		exit_builtin(cmd->cmd[0].arg, cmd, get_freexit(exec, to_free, env));
		// do_builtins(cmd->cmd[0].arg, cmd);
		exit_code = 1;
		return ;
	}
	exit_code = do_builtins(cmd->cmd[0].arg);
}
