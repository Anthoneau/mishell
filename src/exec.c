/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/27 17:26:46 by agoldber         ###   ########.fr       */
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

void	exec_cmd(char *path, char **arg, int *pipe_fd, int n, char **env)
{
	if (pipe_fd)
	{
		
	}
	execve(path, arg, env);
	// free(path);
	// free_array(arg);
	printf("%s: command not found\n", arg[0]); //print temporaire
	exit(127);
}

pid_t	first(int pipe_fd[2], char **env, t_ast *ast)
{
	pid_t	pid;
	char	*path;
	char	**arg;

	path = right_path(ast->content, env);
	if (!path)
	{
		printf("pas de path dans exec\n"); //print temporaire
		exit(1);
	}
	arg = ft_split(ast->content, ' ');
	pid = fork();
	if (pid == -1)
		return (printf("pid = -1\n"));//print temporaire
	if (!pid)
	{
		dup2(pipe_fd[1], 1);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execve(path, arg, env);
		printf("%s: command not found\n", arg[0]); //print temporaire
		exit(127);
	}
	return (pid);
}

pid_t	second(int pipe_fd[2], char **env, t_ast *ast)
{
	pid_t	pid;
	char	*path;
	char	**arg;

	path = right_path(ast->content, env);
	if (!path)
	{
		printf("pas de path dans exec\n"); //print temporaire
		exit(1);
	}
	arg = ft_split(ast->content, ' ');
	pid = fork();
	if (pid == -1)
		return (printf("pid = -1\n"));//print temporaire
	if (!pid)
	{
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execve(path, arg, env);
		printf("%s: command not found\n", arg[0]); //print temporaire
		exit(127);
	}
	return (pid);
}

int	exec_node(t_ast *ast, int *pipe_fd, int n, char **env)
{
	char	*path;
	char	**arg;
	pid_t	pid;
	int		status;

	status = 0;
	path = right_path(ast->content, env);
	if (!path)
		return (printf("pas de path dans exec\n")); //print temporaire
	arg = ft_split(ast->content, ' ');
	pid = fork();
	if (pid == -1)
		return (printf("pid = -1\n"));//print temporaire
	if (!pid)
		exec_cmd(path, arg, env, pipe_fd, n);
	free(path);
	free_array(arg);
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

void	close_pipes(int *pipe_fd, int m, int skip, int skip2)
{
	int	i;

	i = 0;
	while (i <= m)
	{
		if (i != skip && i != skip2)
			close(pipe_fd[i]);
		i++;
	}
}

void	fun(t_ast *node)
{
	if (node->type == PIPE)
	{
		PIPE;
	}
	else if (node->type == REDIR)
	{
		redir;
	}
	else if (node->type == WORD)
	{
		word;
	}
	if (node->left)
	{
		fun(node->left);
	}
	if (node->right)
	{
		fun(node->right);
	}
	return (status);
}

int	exec(t_ast *ast, char **env, int m)
{
	int		pipe_fd[m];
	int		status;
	pid_t	pid;
	pid_t	pid2;
	int		n;

	status = 0;
	// if (ast->type == PIPE)
	// 	exec_pipe();
	// else if (ast->type >= 2 && ast->type != R_HEREDOC)
	// 	exec_redir();
	// if (m >= 2 && pipe(pipe_fd) == -1)
	// 	return (0);
	if (ast->type == PIPE)
	{
		n = 0;
		while (n < (m * 2))
		{
			if (pipe(pipe_fd + n) == -1)
				return (close_pipes(pipe_fd, n, -1, -1), 0);
			n += 2;
		}
		pid = first(pipe_fd, env, ast->left);
		pid2 = second(pipe_fd,env, ast->right);
		if (m > 0)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		waitpid(pid, &status, 0);
		waitpid(pid2, &status, 0);
	}
	else if (ast->type == WORD)
	{
		status = exec_node(ast, pipe_fd, 0, env);
	}
	return (status);
}
