/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/04 17:32:49 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	wait_cmds(int builtin, pid_t *pid, int i_pid, t_cmdin *cmd)
{
	int			j;
	int			status;
	extern int	g_exit_code;

	status = 0;
	j = 0;
	if (!builtin)
	{
		g_exit_code = 130;
		while (j < i_pid)
		{
			waitpid(pid[j], &status, 0);
			if (WIFSIGNALED(status))
				return (signal_g_exit_code(status, cmd));
			j++;
		}
		g_exit_code = WEXITSTATUS(status);
	}
}

int	real_execution(t_exec *exec, t_cmdin **cmd, t_list **env)
{
	if ((*cmd)->num_of_cmds == 1 && is_builtin((*cmd)->cmd[0].arg[0]))
		return (exec_builtins(exec, cmd, env));
	else
	{
		set_signal_action(2);
		exec->pid[exec->i_pid] = fork();
		if (exec->pid[exec->i_pid] == -1)
		{
			free(exec->path);
			print_error(1, "fork", 1, "");
			return (0);
		}
		if (!exec->pid[exec->i_pid])
			child_process(exec, cmd, env);
	}
	return (1);
}

t_exec	init_exec(t_cmdin cmd)
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

void	exec_cmds(t_cmdin *cmd, t_list **env)
{
	t_exec		exec;

	exec = init_exec(*cmd);
	if (!exec.pid)
		return (print_error(1, "malloc", 1, ""));
	while (exec.i < cmd->num_of_cmds)
	{
		exec.path = right_path(cmd->cmd[exec.i].arg[0], env);
		if (!exec.path)
			return (print_error(1, "malloc", 1, ""));
		if (exec.i < cmd->num_of_cmds - 1 && pipe(exec.newpipefd) == -1)
			return (free(exec.path), print_error(1, "pipe", 1, ""));
		if (!real_execution(&exec, &cmd, env))
			return ;
		parent(&exec, cmd);
		exec.i++;
		exec.i_pid++;
	}
	if (exec.oldpipefd != -1)
		close(exec.oldpipefd);
	wait_cmds(exec.builtin, exec.pid, exec.i_pid, cmd);
	free(exec.pid);
}

void	exec(t_ast *ast, t_list **env)
{
	t_cmdin	cmd;

	cmd = get_cmd_array(ast);
	free_ast(ast);
	if (!cmd.cmd)
		return ;
	if (!cmd.cmd->arg)
		return (free(cmd.cmd));
	exec_cmds(&cmd, env);
	free_cmd(&cmd);
}
