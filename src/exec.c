/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/22 23:06:23 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	exit_code;

char	**get_path(char **full_env)
{
	char	*env;
	char	*temp_env;
	char	**res;
	int		i;

	i = 0;
	while (full_env[i])
	{
		if (ft_strnstr(full_env[i], "PATH", 5))
		{
			env = ft_strdup(full_env[i]);
			if (!env)
				return (NULL);
			temp_env = env;
			while (*temp_env != '/')
				temp_env++;
			res = ft_split(temp_env, ':');
			free(env);
			return (res);
		}
		i++;
	}
	return (NULL);
}

char	*get_full_path(char *path, char *content)
{
	char	*temp;
	char	*res;

	if (*content == '/' || *content == '.')
		return (ft_strdup(content));
	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	res = ft_strjoin(temp, content);
	free(temp);
	if (!res)
		return (NULL);
	return (res);
}

char	*get_cmd(char *content)
{
	char	*cmd;
	int		i;

	i = 0;
	while (content[i] && content[i] != ' ')
		i++;
	cmd = malloc(i + 1);
	if (!cmd)
		return (NULL);
	i = 0;
	while (content[i] && content[i] != ' ')
	{
		cmd[i] = content[i];
		i++;
	}
	cmd[i] = '\0';
	return (cmd);
}

t_arr	get_arr(char *content, char **env)
{
	t_arr	arr;

	arr.arr1 = NULL;
	arr.arr2 = NULL;
	if (!content)
		return (arr);
	arr.arr1 = get_cmd(content);
	if (!arr.arr1)
		return (arr);
	arr.arr2 = get_path(env);
	if (!arr.arr2)
		return (free(arr.arr1), arr);
	return (arr);
}

char	*right_path(char *content, char **env)
{
	char	*full_path;
	t_arr	arr;
	int		i;

	full_path = NULL;
	arr = get_arr(content, env);
	if (!content || !arr.arr1 || !arr.arr2)
		return (ft_free(arr.arr1), free_array(arr.arr2), NULL);
	i = 0;
	while (!full_path || access(full_path, R_OK) == -1)
	{
		// printf("full_path : %s\n", full_path);
		if (!arr.arr2[i])
			break ;
		// if (full_path)
		// 	free(full_path);
		ft_free(full_path);
		full_path = get_full_path(arr.arr2[i], arr.arr1);
		if (!full_path)
			return (free(arr.arr1), free_array(arr.arr2), NULL);
		i++;
	}
	free_array(arr.arr2);
	free(arr.arr1);
	return (full_path);
}

void	print_open_error(char *content)
{
	if (errno == ENOENT)
		print_error(1, content, "No such file or directory");
	else if (errno == EISDIR)
		print_error(1, content, "Is a directory");
	else if (errno == EACCES)
		print_error(1, content, "Permission denied");
	else if (errno == EMFILE || errno == ENFILE)
		print_error(1, NULL, "Too many open files");
	else
		print_error(1, NULL, "Error opening file");
}

void	redir_error(t_ast *ast, t_inout *fd)
{
	if (fd->infile != -1)
		close(fd->infile);
	if (fd->outfile != -1)
		close(fd->outfile);
	fd->infile = -2;
	fd->outfile = -2;
	return (print_open_error(ast->content));
}

void	change_redir(t_ast *ast, t_inout *fd)
{
	// if (ast->type == R_INPUT)
	// {
	// 	if (fd->infile != -1)
	// 		close(fd->infile);
	// 	fd->infile = open(ast->content, O_RDONLY, 0777);
	// 	if (fd->infile == -1)
	// 		redir_error(ast, fd);
	// }
	// else if (ast->type == R_TRUNC || ast->type == R_TRUNC_NOCLOBBER)
	// {
	// 	if (fd->outfile != -1)
	// 		close(fd->outfile);
	// 	fd->outfile = open(ast->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// 	if (fd->outfile == -1)
	// 		redir_error(ast, fd);
	// }
	// else if (ast->type == R_APPEND)
	// {
	// 	if (fd->outfile != -1)
	// 		close(fd->outfile);
	// 	fd->outfile = open(ast->content, O_WRONLY | O_CREAT |O_APPEND, 0644);
	// 	if (fd->outfile == -1)
	// 		redir_error(ast, fd);
	// }
	// else if (ast->type == R_HEREDOC)
	// {
	// 	if (fd->infile != -1)
	// 		close(fd->infile);
	// 	fd->infile = dup(ast->fd);
	// 	close(ast->fd);
	// 	if (fd->infile == -1)
	// 		redir_error(ast, fd);
	// }
	if (ast->type == R_INPUT || ast->type == R_HEREDOC)
	{
		if (fd->infile != -1)
			close(fd->infile);
		if (ast->type == R_INPUT)
			fd->infile = open(ast->content, O_RDONLY, 0777);
		else
			fd->infile = dup(ast->fd);
		if (fd->infile == -1)
			redir_error(ast, fd);
	}
	else if (ast->type == R_TRUNC || ast->type == R_TRUNC_NOCLOBBER || ast->type == R_APPEND)
	{
		if (fd->outfile != -1)
			close(fd->outfile);
		if (ast->type == R_APPEND)
			fd->outfile = open(ast->content, O_WRONLY | O_CREAT |O_APPEND, 0644);
		else
			fd->outfile = open(ast->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd->outfile == -1)
			redir_error(ast, fd);
	}
}

t_inout	get_fd(t_ast *ast)
{
	t_inout	fd;

	fd.infile = -1;
	fd.outfile = -1;
	while (ast)
	{
		if (ast->done == 1)
			break ;
		change_redir(ast, &fd);
		if (fd.infile == -2 || fd.outfile == -2)
			return (fd);
		ast->done = 1;
		if (ast->left && ast->left->type >= R_INPUT && ast->left->done == 0)
			ast = ast->left;
		else if (ast->right && ast->right->type >= R_INPUT && ast->right->done == 0)
			ast = ast->right;
		else if (ast->top && ast->top->type != PIPE)
			ast = ast->top;
		else
			break ;
	}
	return (fd);
}

t_ast	*find_new_current(t_ast *node)
{
	t_ast	*left;
	t_ast	*right;

	left = NULL;
	right = NULL;
	if (node->type == WORD)
		return (node);
	if (node->left)
		left = find_new_current(node->left);
	if (node->right)
		right = find_new_current(node->right);
	if (left && left->type == WORD)
		return (left);
	if (right && right->type == WORD)
		return (right);
	return (NULL);
}

int	create_cmds_array(t_cmd_info *cmd, t_ast * ast)
{
	t_ast	*current;
	t_inout	fd;
	int		i;

	current = ast;
	i = 0;
	while (current && i < cmd[0].num_of_cmds)
	{
		fd.infile = -1;
		fd.outfile = -1;
		if (current->type == PIPE)
		{
			while (current->type == PIPE)
			{
				if (current->left && (current->left->done == 0 || current->left->type == PIPE))
					current = current->left;
				else if (current->right && (current->right->done == 0 || current->right->type == PIPE))
					current = current->right;
			}
		}
		if (current->type >= R_INPUT && current->done == 0)
		{
			fd = get_fd(current);
			if (fd.infile == -2 || fd.outfile == -2)
				return (0);
			current->done = 1;
			current = find_new_current(current);
			if (!current)
				return (0);
		}
		if (current->type == WORD && current->done == 0)
		{
			current->done = 1;
			cmd->cmd[i].arg = ft_arrdup(current->arg);
			if (!cmd->cmd[i].arg)
				return (0);
			cmd->cmd[i].fd_in = fd.infile;
			cmd->cmd[i].fd_out = fd.outfile;
		}
		if (cmd->cmd[i].arg)
			i++;
		current = ast;
	}
	return (1);
}

size_t	get_cmd_number(t_ast *ast)
{
	size_t	number;

	number = 0;
	if (ast->type == WORD)
		number++;
	if (ast->left)
		number += get_cmd_number(ast->left);
	if (ast->right)
		number += get_cmd_number(ast->right);
	return (number);
}

t_cmd_info	get_cmd_array(t_ast *ast)
{
	t_cmd_info	cmd;

	cmd.num_of_cmds = get_cmd_number(ast);
	cmd.cmd = ft_calloc(cmd.num_of_cmds + 1, sizeof(t_cmd));
	if (!cmd.cmd)
		return (cmd);
	if (!create_cmds_array(&cmd, ast))
		return (free_cmd(&cmd), cmd);
	return (cmd);
}

void	display_cmds(t_cmd_info cmd)
{
	int	i;

	i = 0;
	while (i < cmd.num_of_cmds)
	{
		printf("%scommand[%d] :%s\n", BPURPLE, i, END);
		if (cmd.cmd[i].content)
			printf("%s\tcontent -- %s%s\n", YELLOW, cmd.cmd[i].content, END);
		if (cmd.cmd[i].arg)
		{
			int j = 0;
			while (cmd.cmd[i].arg[j])
			{
				printf("%s\targ[%d] -- %s\n%s", YELLOW, j, cmd.cmd[i].arg[j], END);
				j++;
			}
		}
		printf("%s\tfd_in   -- %d%s\n", YELLOW, cmd.cmd[i].fd_in, END);
		printf("%s\tfd_out  -- %d%s\n", YELLOW, cmd.cmd[i].fd_out, END);
		i++;
	}
}

int	is_builtin(char *content)
{
	if ((ft_strncmp(content, "echo", 5) == 0) || !ft_strncmp(content, "cd", 3)
		|| !ft_strncmp(content, "pwd", 4) || !ft_strncmp(content, "export", 7)
		|| !ft_strncmp(content, "unset", 6) || !ft_strncmp(content, "env", 4)
		|| !ft_strncmp(content, "exit", 5))
		return (1);
	return (0);
}

int	do_builtins(char **arg, t_cmd_info *cmd)
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
	else if (!ft_strncmp(arg[0], "exit", 5))
		exit_builtin(arg, cmd);
	return (0);
}

void	signal_exit_code(int status, t_cmd_info cmd)
{
	extern int	exit_code;
	int			sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		exit_code = 130;
	}
	else if (sig == SIGQUIT)
	{
		if (cmd.num_of_cmds == 1)
			ft_putstr_fd("Quit (core dumped)\n", 1);
		exit_code = 131;
	}
}

void	wait_cmds(int builtin, pid_t *pid, int i_pid, t_cmd_info cmd)
{
	int			j;
	int			status;
	extern int	exit_code;

	status = 0;
	j = 0;
	exit_code = 130;
	if (!builtin)
	{
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

void	exec_builtins(t_exec *exec, t_cmd_info *cmd, t_free to_free, char **env)
{
	(void)env;
	exec->builtin = 1;
	if (!ft_strncmp(cmd->cmd[0].arg[0], "exit", 5))
	{
		free(exec->path);
		free(exec->pid);
		free_to_free(to_free);
		// free(env);
	}
	exit_code = do_builtins(cmd->cmd[0].arg, cmd);
}

void	child_process(t_exec *exec, t_cmd_info *cmd, t_free to_free, char **env)
{
	if (cmd->num_of_cmds > 1)
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
	if (cmd->cmd[exec->i].fd_in != -1)
	{
		if (dup2(cmd->cmd[exec->i].fd_in, STDIN_FILENO) == -1)
		{
			free(exec->pid);
			free_to_free(to_free);
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
			free_to_free(to_free);
			free(exec->path);
			exit(1);
		}
		close(cmd->cmd[exec->i].fd_out);
	}
	if (exec->newpipefd[0] != -1)
		close(exec->newpipefd[0]);
	if (exec->newpipefd[1] != -1)
		close(exec->newpipefd[1]);
	if (exec->oldpipefd != -1)
		close(exec->oldpipefd);
	free(exec->pid);
	free_to_free(to_free);
	if (cmd->cmd[exec->i].arg[0] && is_builtin(cmd->cmd[exec->i].arg[0]))
	{
		free(exec->path);
		do_builtins(cmd->cmd[exec->i].arg, cmd);
		exit(0);
	}
	else
		execve(exec->path, cmd->cmd[exec->i].arg, env);
	print_error(0, cmd->cmd[exec->i].arg[0], "command not found");
	free(exec->path);
	exit(127);
}

void	parent(t_exec *exec, t_cmd_info *cmd)
{
	free(exec->path);
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

int	real_execution(t_exec *exec, t_cmd_info *cmd, t_free to_free, char **env)
{
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

// void	exec_cmds(t_cmd_info cmd, char **env, t_free to_free)
// {
// 	// pid_t		*pid;
// 	t_exec		exec;
// 	extern int	exit_code;

// 	// exec.i = 0;
// 	// exec.i_pid = 0;
// 	// exec.newpipefd[0] = -1;
// 	// exec.newpipefd[1] = -1;
// 	// exec.oldpipefd = -1;
// 	// exec.builtin = 0;
// 	// exec.pid = malloc(cmd.num_of_cmds * sizeof(pid_t));
// 	// if (!exec.pid)
// 	// {
// 	// 	return (print_error(1, "malloc", "Cannot allocate memory"));
// 	// }
// 	exec = init_exec(cmd);
// 	if (!exec.pid)
// 		return (print_error(1, "malloc", "Cannot allocate memory"));
// 	while (exec.i < cmd.num_of_cmds)
// 	{
// 		// set_signal_action(0);
// 		exec.path = right_path(cmd.cmd[exec.i].arg[0], env);
// 		if (!exec.path)
// 			return (print_error(1, "malloc", "Cannot allocate memory"));
// 		if (exec.i < cmd.num_of_cmds - 1 && pipe(exec.newpipefd) == -1)
// 			return (free(exec.path), print_error(1, "pipe", "Cannot allocate memory"));
// 		if (cmd.num_of_cmds == 1 && is_builtin(cmd.cmd[0].arg[0]))
// 		{
// 			exec.builtin = 1;
// 			if (!ft_strncmp(cmd.cmd[0].arg[0], "exit", 5))
// 			{
// 				free(exec.path);
// 				free(exec.pid);
// 				free_to_free(to_free);
// 				// free(env);
// 			}
// 			exit_code = do_builtins(cmd.cmd[0].arg, &cmd);
// 		}
// 		else
// 		{
// 			set_signal_action(2);
// 			exec.pid[exec.i_pid] = fork();
// 			if (exec.pid[exec.i_pid] == -1)
// 			{
// 				free(exec.path);
// 				print_error(1, "fork", "Cannot allocate memory");
// 				return ;
// 			}
// 			if (!exec.pid[exec.i_pid])
// 			{
// 				if (cmd.num_of_cmds > 1)
// 				{
// 					if (exec.i == 0 && cmd.cmd[exec.i].fd_out == -1)
// 						cmd.cmd[exec.i].fd_out = exec.newpipefd[1];
// 					else if (exec.i == cmd.num_of_cmds - 1 && cmd.cmd[exec.i].fd_in == -1)
// 						cmd.cmd[exec.i].fd_in = exec.oldpipefd;
// 					else if (exec.i > 0 && exec.i < cmd.num_of_cmds - 1)
// 					{
// 						if (cmd.cmd[exec.i].fd_out == -1)
// 							cmd.cmd[exec.i].fd_out = exec.newpipefd[1];
// 						if (cmd.cmd[exec.i].fd_in == -1)
// 							cmd.cmd[exec.i].fd_in = exec.oldpipefd;
// 					}
// 				}
// 				if (cmd.cmd[exec.i].fd_in != -1)
// 				{
// 					if (dup2(cmd.cmd[exec.i].fd_in, STDIN_FILENO) == -1)
// 					{
// 						free(exec.pid);
// 						free_to_free(to_free);
// 						free(exec.path);
// 						exit(1);
// 					}
// 					close(cmd.cmd[exec.i].fd_in);
// 				}
// 				if (cmd.cmd[exec.i].fd_out != -1)
// 				{
// 					if (dup2(cmd.cmd[exec.i].fd_out, STDOUT_FILENO) == -1)
// 					{
// 						free(exec.pid);
// 						free_to_free(to_free);
// 						free(exec.path);
// 						exit(1);
// 					}
// 					close(cmd.cmd[exec.i].fd_out);
// 				}
// 				if (exec.newpipefd[0] != -1)
// 					close(exec.newpipefd[0]);
// 				if (exec.newpipefd[1] != -1)
// 					close(exec.newpipefd[1]);
// 				if (exec.oldpipefd != -1)
// 					close(exec.oldpipefd);
// 				free(exec.pid);
// 				free_to_free(to_free);
// 				if (cmd.cmd[exec.i].arg[0] && is_builtin(cmd.cmd[exec.i].arg[0]))
// 				{
// 					free(exec.path);
// 					do_builtins(cmd.cmd[exec.i].arg, &cmd);
// 					exit(0);
// 				}
// 				else
// 					execve(exec.path, cmd.cmd[exec.i].arg, env);
// 				print_error(0, cmd.cmd[exec.i].arg[0], "command not found");
// 				free(exec.path);
// 				exit(127);
// 			}
// 		}
// 		free(exec.path);
// 		if (exec.newpipefd[1] != -1)
// 			close(exec.newpipefd[1]);
// 		if (exec.oldpipefd != -1)
// 			close(exec.oldpipefd);
// 		if (exec.newpipefd[0] != -1)
// 		{
// 			exec.oldpipefd = dup(exec.newpipefd[0]);
// 			close(exec.newpipefd[0]);
// 			exec.newpipefd[0] = -1;
// 		}
// 		if (cmd.cmd[exec.i].fd_in != -1)
// 			close(cmd.cmd[exec.i].fd_in);
// 		if (cmd.cmd[exec.i].fd_out != -1)
// 			close(cmd.cmd[exec.i].fd_out);
// 		exec.i++;
// 		exec.i_pid++;
// 	}
// 	if (exec.oldpipefd != -1)
// 		close(exec.oldpipefd);
// 	wait_cmds(exec.builtin, exec.pid, exec.i_pid, cmd);
// 	free(exec.pid);
// }

void	exec(t_ast *ast, char **env, t_free to_free)
{
	t_cmd_info	cmd;

	cmd = get_cmd_array(ast);
	if (!cmd.cmd || !cmd.cmd->arg)
		return ;//temp print
	// display_cmds(cmd);
	exec_cmds(cmd, env, to_free);
	free_cmd(&cmd);
}
