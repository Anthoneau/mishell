/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/29 16:32:53 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	exec(t_ast *ast)
// {
// 	pid_t	pid;
// 	int		status;

// 	while (ast)
// 	{
// 		if (pipe)
// 			exec_pipe();
// 		if (redir)
// 			exec_redir();
// 		if (word)
// 			exec_cmd();
// 	}
// 	return (WEXITSTATUS(status));
// }

// int	exec(t_ast *ast)
// {
// 	if (ast->left)
// 		recursive(ast->left);
// 	if (ast->right)
// 		recursive(ast->right);
// }

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

	if (*content == '/')
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

char	*right_path(char *content, char **env)
{
	char	*full_path;
	char	*cmd;
	char	**path;
	int		i;

	full_path = NULL;
	if (!content)
		return (NULL);
	cmd = get_cmd(content);
	if (!cmd)
		return (NULL);
	path = get_path(env);
	if (!path)
		return (free(cmd), NULL);
	i = 0;
	while (access(full_path, R_OK) == -1)
	{
		if (!path[i])
			break ;
		if (full_path)
			free(full_path);
		full_path = get_full_path(path[i], cmd);
		if (!full_path)
			return (free(cmd), free_array(path), NULL);
		i++;
	}
	free_array(path);
	free(cmd);
	return (full_path);
}

// void	exec_cmd(char *path, char **arg, int *pipe_fd, int n, char **env)
// {
// 	(void)pipe_fd;
// 	(void)n;
// 	execve(path, arg, env);
// 	free(path);
// 	free_array(arg);
// 	printf("%s: command not found\n", arg[0]); //print temporaire
// 	exit(127);
// }

// pid_t	first(int pipe_fd[2], char **env, t_ast *ast)
// {
// 	pid_t	pid;
// 	char	*path;
// 	char	**arg;

// 	path = right_path(ast->content, env);
// 	if (!path)
// 	{
// 		printf("pas de path dans exec\n"); //print temporaire
// 		exit(1);
// 	}
// 	arg = ft_split(ast->content, ' ');
// 	pid = fork();
// 	if (pid == -1)
// 		return (printf("pid = -1\n"));//print temporaire
// 	if (!pid)
// 	{
// 		dup2(pipe_fd[1], 1);
// 		close(pipe_fd[0]);
// 		close(pipe_fd[1]);
// 		execve(path, arg, env);
// 		printf("%s: command not found\n", arg[0]); //print temporaire
// 		exit(127);
// 	}
// 	return (pid);
// }

// pid_t	second(int pipe_fd[2], char **env, t_ast *ast)
// {
// 	pid_t	pid;
// 	char	*path;
// 	char	**arg;

// 	path = right_path(ast->content, env);
// 	if (!path)
// 	{
// 		printf("pas de path dans exec\n"); //print temporaire
// 		exit(1);
// 	}
// 	arg = ft_split(ast->content, ' ');
// 	pid = fork();
// 	if (pid == -1)
// 		return (printf("pid = -1\n"));//print temporaire
// 	if (!pid)
// 	{
// 		dup2(pipe_fd[0], 0);
// 		close(pipe_fd[0]);
// 		close(pipe_fd[1]);
// 		execve(path, arg, env);
// 		printf("%s: command not found\n", arg[0]); //print temporaire
// 		exit(127);
// 	}
// 	return (pid);
// }

// int	exec_node(t_ast *ast, int *pipe_fd, int n, char **env)
// {
// 	char	*path;
// 	char	**arg;
// 	pid_t	pid;
// 	int		status;

// 	status = 0;
// 	path = right_path(ast->content, env);
// 	if (!path)
// 		return (printf("pas de path dans exec\n")); //print temporaire
// 	arg = ft_split(ast->content, ' ');
// 	if (!arg)
// 		return (free(path), printf("probleme avec arg\n"));//print temp
// 	pid = fork();
// 	if (pid == -1)
// 		return (printf("pid = -1\n"));//print temporaire
// 	if (!pid)
// 		exec_cmd(path, arg, pipe_fd, n, env);
// 	free(path);
// 	free_array(arg);
// 	if (pipe_fd)
// 	{
// 		close(pipe_fd[0]);
// 		close(pipe_fd[1]);
// 	}
// 	waitpid(pid, &status, 0);
// 	return (WEXITSTATUS(status));
// }

// void	close_pipes(int *pipe_fd, int m, int skip, int skip2)
// {
// 	int	i;

// 	i = 0;
// 	while (i <= m)
// 	{
// 		if (i != skip && i != skip2)
// 			close(pipe_fd[i]);
// 		i++;
// 	}
// }

// void	fun(t_ast *node)
// {
// 	if (node->type == PIPE)
// 	{
// 		PIPE;
// 	}
// 	else if (node->type == REDIR)
// 	{
// 		redir;
// 	}
// 	else if (node->type == WORD)
// 	{
// 		word;
// 	}
// 	if (node->left)
// 	{
// 		fun(node->left);
// 	}
// 	if (node->right)
// 	{
// 		fun(node->right);
// 	}
// 	return (status);
// }

int	print_open_error(char *content)
{
	if (errno == EISDIR)
		printf("minishell: %s: Is a directory\n", content);
	else if (errno == EACCES)
		printf("minishell: %s: Permission denied\n", content);
	else if (errno == EMFILE || errno == ENFILE)
		printf("minishell: Too many open files\n");
	else
		printf("minishell: Error opening file\n");
	return (0);
}

int	change_redir(t_ast *ast, t_cmd *cmd)
{
	int	fd;

	fd = 0;
	printf("-------------change redir-------------\n");
	if (ast->type == R_INPUT)
	{
		printf("input trouve\n");
		printf("open de %s\n", ast->content);
		fd = open(ast->content, O_RDONLY, 0777);
		if (fd == -1)
		{
			// if (errno == EACCES)
			// 	printf("minishell: %s: Permission denied\n", ast->content);
			// else if (errno == EMFILE || errno == ENFILE)
			// 	printf("minishell: Too many open files\n");
			// else
			// 	printf("minishell: Error opening file\n");
			// return (0);
			return (print_open_error(ast->content));
		}
		printf("ouverture reussie\n");
		if (cmd->fd_in != -1)
		{
			printf("fd_in != -1 donc on ferme d'abord\n");
			close(cmd->fd_in);
		}
		cmd->fd_in = fd;
		printf("on assigne %d a fd_in\n", fd);
		// sleep(1);
	}
	else if (ast->type == R_TRUNC || ast->type == R_TRUNC_NOCLOBBER)
	{
		printf("trunc trouve\n");
		printf("open de %s\n", ast->content);
		fd = open(ast->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			// printf("error\n");
			// if (errno == EISDIR)
			// 	printf("minishell: %s: Is a directory\n", ast->content);
			// else if (errno == EACCES)
			// 	printf("minishell: %s: Permission denied\n", ast->content);
			// else if (errno == EMFILE || errno == ENFILE)
			// 	printf("minishell: Too many open files\n");
			// else
			// 	printf("minishell: Error opening file\n");
			// return (0);
			return (print_open_error(ast->content));
		}
		printf("ouverture reussie\n");
		if (cmd->fd_out != -1)
		{
			printf("fd_out != -1 donc on ferme\n");
			close(cmd->fd_out);
		}
		printf("on assigne %d\n", fd);
		cmd->fd_out = fd;
	}
	else if (ast->type == R_APPEND)
	{
		printf("append trouve\n");
		printf("open de %s\n", ast->content);
		fd = open(ast->content, O_WRONLY | O_CREAT |O_APPEND, 0644);
		if (fd == -1)
		{
			// if (errno == EISDIR)
			// 	printf("minishell: %s: Is a directory\n", ast->content);
			// else if (errno == EACCES)
			// 	printf("minishell: %s: Permission denied\n", ast->content);
			// else if (errno == EMFILE || errno == ENFILE)
			// 	printf("minishell: Too many open files\n");
			// else
			// 	printf("minishell: Error opening file\n");
			// return (0);
			return (print_open_error(ast->content));
		}
		printf("ouverture reussie\n");
		if (cmd->fd_out != -1)
		{
			printf("fd_out != -1 donc on ferme\n");
			close(cmd->fd_out);
		}
		printf("on assigne %d\n", fd);
		cmd->fd_out = fd;
	}
	return (1);
}

pid_t	exec_cmd(t_cmd *cmd, char **env)
{
// int	exec_node(t_ast *ast, int *pipe_fd, int n, char **env)
	char	*path;
	char	**arg;
	pid_t	pid;

	path = right_path(cmd->content, env);
	if (!path)
	{
		printf("pas de path dans exec\n"); //print temporaire
		return (-1);
	}
	printf("path = %s\n", path);
	// sleep(1);
	arg = ft_split(cmd->content, ' ');
	if (!arg)
	{
		printf("probleme avec arg\n");
		return (free(path), -1);//print temp
	}
	printf("split de arg\n");
	// sleep(1);
	pid = fork();
	printf("fork\n");
	if (pid == -1)
	{
		printf("pid = -1\n");
		return (free(path), free_array(arg), -1);//print temporaire
	}
	if (!pid)
	{
		printf("on est dans le child\n");
		if (cmd->fd_in != -1)
		{
			printf("fd in pas par defaut : %d\n", cmd->fd_in);
			dup2(cmd->fd_in, 0);
			if (cmd->fd_in != -1)
				close(cmd->fd_in);
		}
		if (cmd->fd_out != -1)
		{
			printf("fd out pas par defaut : %d\n", cmd->fd_out);
			// sleep(1);
			dup2(cmd->fd_out, 1);
			printf("hein\n");
			if (cmd->fd_out != -1)
				close(cmd->fd_out);
			printf("hein\n");
		}
		printf("-------------execve-------------\n");
		execve(path, arg, env);
		free(path);
		free_array(arg);
		printf("%s: command not found\n", arg[0]); //print temporaire
		exit(127);
	}
	printf("free de path et arg\n");
	free(path);
	free_array(arg);
	return (pid);
}

int	exec_node_ast(t_ast *ast, t_cmd *cmd, int in, int out, char **env)
{
	int		status;
	pid_t	pid;

	printf("-------------exec node ast-------------\n");
	// sleep(1);
	status = 0;
	printf("check du noeud\n");
	if (!ast)
	{
		printf("pas de noeud\n");
		return (0);
	}
	printf("check de la commande\n");
	if (!cmd)
	{
		printf("malloc de cmd\n");
		// sleep(1);
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
			return (printf("probleme de malloc\n")); //print temp
		cmd->content = NULL;
		cmd->fd_in = -1;
		cmd->fd_out = -1;
	}
	printf("cmd existe\n");
	if (cmd && cmd->content)
		printf("cmd->content = %s\n", cmd->content);
	printf("test\n");
	printf("cmd->fd_in = %d\n", cmd->fd_in);
	printf("test\n");
	printf("cmd->fd_out = %d\n", cmd->fd_out);
	printf("test\n");
	// sleep(1);
	if (ast->type == PIPE)
	{
		printf("on trouve un pipe\n");
		// sleep(1);
		int	pipefd[2];
	
		if (pipe(pipefd) == -1)
			return (printf("probleme de pipe\n"));
		printf("creation du pipe\n");
		// sleep(1);
		if (ast->left)
		{
			printf("on envoie ast->left a gauche depuis pipe\n");
			// if (ast->left && ast->left->type)
				printf("ast->left est de type = %d\n", ast->left->type);
			// sleep(1);
			printf("exec_node_ast retourne : %d\n", exec_node_ast(ast->left, cmd, in, pipefd[1], env));
			printf("revenu\n");
			// sleep(1);
			// if (cmd && cmd->content)
			// 	free(cmd->content);
			// if (cmd)
			// 	free(cmd);
			printf("-------------revenu dans la fonction du pipe-------------\n");
		}
		if (ast->right)
		{
			printf("on envoie ast->right a droite depuis pipe\n");
			// if (ast->right->type)
				printf("ast->right est de type = %d\n", ast->right->type);
			// sleep(1);
			exec_node_ast(ast->right, cmd, pipefd[0], out, env);
			if (cmd && cmd->content)
				free(cmd->content);
			if (cmd)
				free(cmd);
			printf("-------------revenu dans la fonction du pipe-------------\n");
		}
		printf("on ferme les pipe[1] et pipe[0]\n");
		close(pipefd[0]);
		close(pipefd[1]);
	}
	else if (ast->type >= R_INPUT && ast->type != R_HEREDOC)
	{
		printf("on trouve une redir\n");
		if (!change_redir(ast, cmd))
		{
			printf("les redirections ont foires\n");
			if (cmd && cmd->content)
				free(cmd);
			if (cmd)
				free(cmd);
			return (1);
		}
		if (ast->left)
		{
			printf("on envoie ast->left a gauche depuis redir\n");
			// if (ast->left && ast->left->type)
				printf("ast->left est de type = %d\n", ast->left->type);
			// sleep(1);
			exec_node_ast(ast->left, cmd, in, out, env);
			// if (cmd && cmd->content)
			// 	free(cmd->content);
			// if (cmd)
			// 	free(cmd);
			printf("-------------revenu dans la fonction de la redir-------------\n");
		}
		if (ast->right)
		{
			printf("on envoie ast->right a droite depuis redir\n");
			// if (ast->right && ast->right->type)
			printf("ast->right est de type = %d\n", ast->right->type);
			// sleep(1);
			exec_node_ast(ast->right, cmd, in, out, env);
			// if (cmd && cmd->content)
			// 	free(cmd->content);
			// if (cmd)
			// 	free(cmd);
			printf("-------------revenu dans la fonction de la redir-------------\n");
		}
	}
	else if (ast->type == WORD)
	{
		printf("strdup de %s\n", ast->content);
		if (cmd->content)
			free(cmd->content);
		cmd->content = ft_strdup(ast->content);
		if (!cmd->content)
			return (printf("probleme de malloc\n")); //print temp
		if (in != -1 && cmd->fd_in == -1)
		{
			printf("in = %d et cmd->fd_in == -1\n", in);
			cmd->fd_in = in;
		}
		if (out != -1 && cmd->fd_out == -1)
		{
			printf("out = %d et cmd->fd_out == -1\n", out);
			cmd->fd_out = out;
		}
		// sleep(1);
		pid = exec_cmd(cmd, env);
		// sleep(1);
		if (pid == -1)
		{
			printf("probleme de fork, on free\n");
			free(cmd->content);
			free(cmd);
			return (0);
		}
		printf("pid wait = %d\n", pid);
		waitpid(pid, &status, 0);
		printf("on a fini d'attendre\n");
		// sleep(1);
		printf("cmd->fdin : %d\ncmd->fdout : %d\n", cmd->fd_in, cmd->fd_out);
		if (cmd->fd_in != -1)
			close(cmd->fd_in);
		if (cmd->fd_out != -1)
			close(cmd->fd_out);
		if (in != -1)
			close(in);
		if (out != -1)
			close(out);
		cmd->fd_in = -1;
		cmd->fd_out = -1;
		// sleep(1);
		// if (cmd && cmd->content)
		// 	free(cmd->content);
		// if (cmd)
		// 	free(cmd);
		printf("tout est free et close\n");
	}
	else
	{
		printf("on free tout\n");
		if (cmd && cmd->content)
		{
			printf("cmd->content = %s\n", cmd->content);
			free(cmd->content);
		}
		if (cmd)
			free(cmd);
	}
	printf("fin de exec_node_ast\n");
	printf("ce qu'on retourne : %d\n", WEXITSTATUS(status));
	return (WEXITSTATUS(status));
}

int	exec(t_ast *ast, char **env, int m)
{
	int		pipe_fd[m];
	int		status;
	// pid_t	pid;
	// pid_t	pid2;
	// int		n;

	status = 0;
	(void)pipe_fd;
	// if (ast->type == PIPE)
	// 	exec_pipe();
	// else if (ast->type >= 2 && ast->type != R_HEREDOC)
	// 	exec_redir();
	// if (m >= 2 && pipe(pipe_fd) == -1)
	// 	return (0);
	// if (ast->type == PIPE)
	// {
	// 	n = 0;
	// 	while (n < (m * 2))
	// 	{
	// 		if (pipe(pipe_fd + n) == -1)
	// 			return (close_pipes(pipe_fd, n, -1, -1), 0);
	// 		n += 2;
	// 	}
	// 	pid = first(pipe_fd, env, ast->left);
	// 	pid2 = second(pipe_fd,env, ast->right);
	// 	if (m > 0)
	// 	{
	// 		close(pipe_fd[0]);
	// 		close(pipe_fd[1]);
	// 	}
	// 	waitpid(pid, &status, 0);
	// 	waitpid(pid2, &status, 0);
	// }
	// if (ast->type == WORD)
	// {
	// 	status = exec_node(ast, pipe_fd, 0, env);
	// }
	printf("on entre dans exec_node_ast\n");
	status = exec_node_ast(ast, NULL, -1, -1, env);
	return (status);
}
