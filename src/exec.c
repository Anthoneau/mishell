/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/19 16:02:23 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_cmd(t_cmd_info *cmd)
{
	int	i;

	if (!cmd || !cmd->cmd)
		return ;
	i = 0;
	while (cmd->cmd && i < cmd->num_of_cmds)
	{
		if (cmd->cmd[i].content) //ligne 745
			free(cmd->cmd[i].content);
		if (cmd->cmd[i].arg)
			free_array(cmd->cmd[i].arg);
		i++;
	}
	free(cmd->cmd);
	cmd->cmd = NULL;
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

int	is_buitin(char *content)
{
	if (!ft_strncmp(content, "echo", 4) || !ft_strncmp(content, "cd", 4)
		|| !ft_strncmp(content, "pwd", 4) || !ft_strncmp(content, "export", 4)
		|| !ft_strncmp(content, "unset", 4) || !ft_strncmp(content, "env", 4)
		|| !ft_strncmp(content, "exit", 4))
		return (1);
	return (0);
}

int	do_builtins(char **arg)
{
	if (!ft_strncmp(arg[0], "echo", 4))
		return (printf("echo\n"));
	else if (!ft_strncmp(arg[0], "cd", 4))
		return (printf("cd\n"));
	else if (!ft_strncmp(arg[0], "pwd", 4))
		return (printf("pwd\n"));
	else if (!ft_strncmp(arg[0], "export", 4))
		return (printf("export\n"));
	else if (!ft_strncmp(arg[0], "unset", 4))
		return (printf("unset\n"));
	else if (!ft_strncmp(arg[0], "env", 4))
		return (printf("env\n"));
	else if (!ft_strncmp(arg[0], "exit", 4))
		exit_builtin(arg);
	return (0);
}

void	ah(int signal)
{
	extern int exit_code;

	if (signal == SIGINT)
		exit_code = 130;
}

void	set_signal_exec(void)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_handler = &ah;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}

void	exec_cmds(t_cmd_info cmd, char **env, t_free to_free)
{
	char		*path;
	pid_t		*pid;
	int			i_pid;
	int			i;
	int			newpipefd[2];
	int			oldpipefd;
	int			status;
	int			builtin;
	extern int	exit_code;

	i = 0;
	i_pid = 0;
	status = 0;
	newpipefd[0] = -1;
	newpipefd[1] = -1;
	oldpipefd = -1;
	builtin = 0;
	pid = malloc(cmd.num_of_cmds * sizeof(pid_t));
	if (!pid)
	{
		print_error(1, "malloc", "Cannot allocate memory");
		return ;
	}
	while (i < cmd.num_of_cmds) //si execve foire, il faut free les tokens, l'ast, etc...
	{
		path = right_path(cmd.cmd[i].arg[0], env);
		if (!path)
		{
			print_error(1, "malloc", "Cannot allocate memory");
			return ;
		}
		if (i < cmd.num_of_cmds - 1 && pipe(newpipefd) == -1) //line 1039
		{
			free(path);
			print_error(1, "pipe", "Cannot allocate memory");
			return ;
		}
		if (cmd.num_of_cmds == 1 && is_buitin(cmd.cmd[0].arg[0]))
		{
			builtin = 1;
			exit_code = do_builtins(cmd.cmd[0].arg);
		}
		else
		{
			pid[i_pid] = fork();
			if (pid[i_pid] == -1)
			{
				free(path);
				print_error(1, "fork", "Cannot allocate memory");
				return ;
			}
			if (!pid[i_pid])
			{
				set_signal_exec();
				if (cmd.num_of_cmds > 1)
				{
					if (i == 0 && cmd.cmd[i].fd_out == -1)
						cmd.cmd[i].fd_out = newpipefd[1];
					else if (i == cmd.num_of_cmds - 1 && cmd.cmd[i].fd_in == -1)
						cmd.cmd[i].fd_in = oldpipefd;
					else if (i > 0 && i < cmd.num_of_cmds - 1)
					{
						if (cmd.cmd[i].fd_out == -1)
							cmd.cmd[i].fd_out = newpipefd[1];
						if (cmd.cmd[i].fd_in == -1)
							cmd.cmd[i].fd_in = oldpipefd;
					}
				}
				if (cmd.cmd[i].fd_in != -1)
				{
					if (dup2(cmd.cmd[i].fd_in, STDIN_FILENO) == -1)
					{
						free(pid);
						free_to_free(to_free);
						free(path);
						exit(1);
					}
					close(cmd.cmd[i].fd_in);
				}
				if (cmd.cmd[i].fd_out != -1)
				{
					if (dup2(cmd.cmd[i].fd_out, STDOUT_FILENO) == -1)
					{
						free(pid);
						free_to_free(to_free);
						free(path);
						exit(1);
					}
					close(cmd.cmd[i].fd_out);
				}
				if (newpipefd[0] != -1)
					close(newpipefd[0]);
				if (newpipefd[1] != -1)
					close(newpipefd[1]);
				if (oldpipefd != -1)
					close(oldpipefd);
				free(pid);
				free_to_free(to_free);
				// if ()
				execve(path, cmd.cmd[i].arg, env);
				print_error(0, cmd.cmd[i].arg[0], "command not found");
				free(path);
				exit(127);
			}
		}		
		free(path);
		if (newpipefd[1] != -1)
			close(newpipefd[1]);
		if (oldpipefd != -1)
			close(oldpipefd);
		if (newpipefd[0] != -1)
		{
			oldpipefd = dup(newpipefd[0]);
			close(newpipefd[0]);
			newpipefd[0] = -1;
		}
		if (cmd.cmd[i].fd_in != -1)
			close(cmd.cmd[i].fd_in);
		if (cmd.cmd[i].fd_out != -1)
			close(cmd.cmd[i].fd_out);
		i++;
		i_pid++;
	}
	if (oldpipefd != -1)
		close(oldpipefd);
	int j = 0;
	if (!builtin)
	{
		while (j < i_pid)
		{
			waitpid(pid[j], &status, 0);
			j++;
		}
		exit_code = WEXITSTATUS(status);
	}
	free(pid);
}

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
