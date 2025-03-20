/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:15:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/19 19:33:09 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_redirection(t_exec *exec, t_cmdin *cmd)
{
	if (exec->i == 0 && cmd->cmd[exec->i].fd_out == -1)
		cmd->cmd[exec->i].fd_out = exec->newpipefd[1];
	else if (exec->i == cmd->num_of_cmds - 1 && cmd->cmd[exec->i].fd_in == -1)
		cmd->cmd[exec->i].fd_in = exec->oldpipefd;
	else if (exec->i > 0 && exec->i < cmd->num_of_cmds - 1)
	{
		if (cmd->cmd[exec->i].fd_out == -1)
			cmd->cmd[exec->i].fd_out = exec->newpipefd[1];
		if (cmd->cmd[exec->i].fd_in == -1)
			cmd->cmd[exec->i].fd_in = exec->oldpipefd;
	}
}

void	fdin_fdout(t_exec *exec, t_cmdin *cmd)
{
	if (cmd->cmd[exec->i].fd_in != -1)
	{
		if (dup2(cmd->cmd[exec->i].fd_in, STDIN_FILENO) == -1)
		{
			free(exec->pid);
			free(exec->path);
			exit(1);
		}
		close(cmd->cmd[exec->i].fd_in);
	}
	if (cmd->cmd[exec->i].fd_out != -1)
	{
		if (dup2(cmd->cmd[exec->i].fd_out, STDOUT_FILENO) == -1)
		{
			free(exec->pid);
			free(exec->path);
			exit(1);
		}
		close(cmd->cmd[exec->i].fd_out);
	}
}

void	child_process(t_exec *exec, t_cmdin **cmd, t_list **env)
{
	int	nbr;

	if ((*cmd)->num_of_cmds > 1)
		pipe_redirection(exec, *cmd);
	fdin_fdout(exec, *cmd);
	if (exec->newpipefd[0] != -1)
		close(exec->newpipefd[0]);
	if (exec->newpipefd[1] != -1)
		close(exec->newpipefd[1]);
	if (exec->oldpipefd != -1)
		close(exec->oldpipefd);
	ft_free(exec->pid);
	if ((*cmd)->cmd[exec->i].arg[0] && is_builtin((*cmd)->cmd[exec->i].arg[0]))
	{
		builtins_child(exec, cmd, env);
		free_in_child(exec, cmd, env);
		exit(0);
	}
	else if (access((*cmd)->cmd[exec->i].arg[0], X_OK) == 0 || exec->path)
		ft_execve(exec->path, (*cmd)->cmd[exec->i].arg, get_env(*env));
	nbr = get_exit_code((*cmd)->cmd[exec->i].arg[0]);
	free_in_child(exec, cmd, env);
	exit(nbr);
}

void	parent(t_exec *exec, t_cmdin *cmd)
{
	ft_free(exec->path);
	if (exec->newpipefd[1] != -1)
		close(exec->newpipefd[1]);
	if (exec->oldpipefd != -1)
		close(exec->oldpipefd);
	if (exec->newpipefd[0] != -1)
	{
		exec->oldpipefd = dup(exec->newpipefd[0]);
		close(exec->newpipefd[0]);
		exec->newpipefd[0] = -1;
	}
	if (cmd->cmd[exec->i].fd_in != -1)
		close(cmd->cmd[exec->i].fd_in);
	if (cmd->cmd[exec->i].fd_out != -1)
		close(cmd->cmd[exec->i].fd_out);
}
