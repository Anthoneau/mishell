/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/24 17:50:16 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_exit_code;

void	wait_cmds(int builtin, pid_t *pid, int i_pid, t_cmd_info cmd)
{
	int			j;
	int			status;

	status = 0;
	j = 0;
	if (!builtin)
	{
		exit_code = 130;
		while (j < i_pid)
		{
			waitpid(pid[j], &status, 0);
			if (WIFSIGNALED(status))
				return (signal_exit_code(status, cmd));
			j++;
		}
		exit_code = WEXITSTATUS(status);
	}
}

int	real_execution(t_exec *exec, t_cmd_info *cmd, t_free to_free, char **env)
{
	// if (cmd->num_of_cmds == 1 && !ft_strncmp(cmd->cmd[0].arg[0], "exit", 5))
	// {
	// 	do_builtins(cmd->cmd[0].arg, cmd);
	// 	exit_code = 1;
	// }
	if (cmd->num_of_cmds == 1 && is_builtin(cmd->cmd[0].arg[0]))
		exec_builtins(exec, cmd, to_free, env);
	else
	{
		set_signal_action(2);
		exec->pid[exec->i_pid] = fork();
		if (exec->pid[exec->i_pid] == -1)
		{
			free(exec->path);
			print_error(1, "fork", "Cannot allocate memory");
			return (0);
		}
		if (!exec->pid[exec->i_pid])
			child_process(exec, cmd, to_free, env);
	}
	return (1);
}

t_exec	init_exec(t_cmd_info cmd)
{
	t_exec	exec;

	exec.i = 0;
	exec.i_pid = 0;
	exec.newpipefd[0] = -1;
	exec.newpipefd[1] = -1;
	exec.oldpipefd = -1;
	exec.builtin = 0;
	exec.pid = malloc(cmd.num_of_cmds * sizeof(pid_t));
	return (exec);
}

void	exec_cmds(t_cmd_info cmd, char **env, t_free to_free)
{
	t_exec		exec;

	exec = init_exec(cmd);
	if (!exec.pid)
		return (print_error(1, "malloc", "Cannot allocate memory"));
	while (exec.i < cmd.num_of_cmds)
	{
		exec.path = right_path(cmd.cmd[exec.i].arg[0], env);
		if (!exec.path)
			return (print_error(1, "malloc", "Cannot allocate memory"));
		if (exec.i < cmd.num_of_cmds - 1 && pipe(exec.newpipefd) == -1)
			return (free(exec.path), print_error(1, "pipe", "Cannot allocate memory"));
		if (!real_execution(&exec, &cmd, to_free, env))
			return ;
		parent(&exec, &cmd);
		exec.i++;
		exec.i_pid++;
	}
	if (exec.oldpipefd != -1)
		close(exec.oldpipefd);
	wait_cmds(exec.builtin, exec.pid, exec.i_pid, cmd);
	free(exec.pid);
}

void	exec(t_ast *ast, char **env, t_free to_free)
{
	t_cmd_info	cmd;

	cmd = get_cmd_array(ast);
	if (!cmd.cmd || !cmd.cmd->arg)
		return ;
	// display_cmds(cmd);
	exec_cmds(cmd, env, to_free);
	free_cmd(&cmd);
}
