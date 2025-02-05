/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/05 11:59:37 by agoldber         ###   ########.fr       */
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
	while (!full_path || access(full_path, R_OK) == -1)
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

int	print_open_error(char *content)
{
	if (errno == ENOENT)
		// printf("minishell: %s: No such file or directory\n", content);
		print_error_message(1, content, "No such file or directory");
	else if (errno == EISDIR)
		print_error_message(1, content, "Is a directory");
		// printf("minishell: %s: Is a directory\n", content);
	else if (errno == EACCES)
		print_error_message(1, content, "Permission denied");
		// printf("minishell: %s: Permission denied\n", content);
	else if (errno == EMFILE || errno == ENFILE)
		print_error_message(1, NULL, "Too many open files");
		// printf("minishell: Too many open files\n");
	else
		print_error_message(1, NULL, "Error opening file");
		// printf("minishell: Error opening file\n");
	return (0);
}

// size_t	get_len_to_close(int **to_close)
// {
// 	size_t	i;

// 	i = 0;
// 	while (to_close[i])
// 		i++;
// 	return (i);
// }

// int	add_pipe_to_close(int newpipefd[2], int **to_close)
// {
// 	int	**temp;
// 	int	i;

// 	temp = NULL;
// 	i = 0;
// 	if (!to_close)
// 	{
// 		printf("to_close n'existe pas\n");
// 		to_close = malloc(sizeof(int *));
// 		if (!to_close)
// 			return (0);
// 		*to_close = newpipefd;
// 		printf("to_close a newpipe en premiere valeur\n");
// 	}
// 	else
// 	{
// 		printf("to_close existe\n");
// 		temp = malloc(get_len_to_close(to_close) * sizeof(int *));
// 		if (!temp)
// 			return (0);
// 		while (to_close[i])
// 		{
// 			temp[i] = to_close[i];
// 			i++;
// 		}
// 		temp[i] = newpipefd;
// 		free(to_close);
// 		to_close = temp;
// 	}
// 	return (1);
// }

// void	close_all_pipes(int **to_close)
// {
// 	int	i;

// 	i = 0;
// 	if (!to_close)
// 	{
// 		printf("to_close existe pas\n");
// 		return ;
// 	}
// 	while (to_close[i])
// 	{
// 		fprintf(stderr, "pipefd[0] : %d\n", fcntl(*to_close[0], F_GETFD));
// 		fprintf(stderr, "pipefd[1] : %d\n", fcntl(*to_close[1], F_GETFD));
// 		close(to_close[i][0]);
// 		close(to_close[i][1]);
// 		fprintf(stderr, "pipefd[0] : %d\n", fcntl(*to_close[0], F_GETFD));
// 		fprintf(stderr, "pipefd[1] : %d\n", fcntl(*to_close[1], F_GETFD));
// 		i++;
// 	}
// 	free(to_close);
// }

// pid_t	exec_cmd(t_cmd *cmd, char **env, int pipefd[2], int in, int out, int **to_close)
// {
// 	char	*path;
// 	char	**arg;
// 	pid_t	pid;

// 	path = right_path(cmd->content, env);
// 	if (!path)
// 	{
// 		printf("pas de path dans exec\n"); //print temporaire
// 		return (-1);
// 	}
// 	printf("path = %s\n", path);
// 	// sleep(1);
// 	arg = ft_split(cmd->content, ' ');
// 	if (!arg)
// 	{
// 		printf("probleme avec arg\n");
// 		return (free(path), -1);//print temp
// 	}
// 	printf("split de arg\n");
// 	// sleep(1);
// 	pid = fork();
// 	printf("fork\n");
// 	if (pid == -1)
// 	{
// 		printf("pid = -1\n");
// 		return (free(path), free_array(arg), -1);//print temporaire
// 	}
// 	if (!pid)
// 	{
// 		printf("on est dans le child\n");
// 		if (in != -1)
// 		{
// 			dup2(in, STDIN_FILENO);
// 			close(in);
// 			if (cmd->fd_in != -1)
// 				close(cmd->fd_in);
// 		}
// 		else if (cmd->fd_in != -1)
// 		{
// 			dup2(cmd->fd_in, STDIN_FILENO);
// 			close(cmd->fd_in);
// 		}
// 		if (out != -1)
// 		{
// 			dup2(out, STDOUT_FILENO);
// 			close(out);
// 			if (cmd->fd_out != -1)
// 				close(cmd->fd_out);
// 		}
// 		else if (cmd->fd_out != -1)
// 		{
// 			dup2(cmd->fd_out, STDOUT_FILENO);
// 			close(cmd->fd_out);
// 		}
// 		if (pipefd)
// 		{
// 			close(pipefd[0]);
// 			close(pipefd[1]);
// 		}
// 		if (to_close)
// 			close_all_pipes(to_close);
// 		// fprintf(stderr, "in : %d\n", fcntl(in, F_GETFD));
// 		// fprintf(stderr, "out : %d\n", fcntl(out, F_GETFD));
// 		// fprintf(stderr, "cmd->fd_in : %d\n", fcntl(cmd->fd_in, F_GETFD));
// 		// fprintf(stderr, "cmd->fd_out : %d\n", fcntl(cmd->fd_out, F_GETFD));
// 		// if (pipefd)
// 		// {
// 		// 	printf("pipefd[0] : %d\n", fcntl(pipefd[0], F_GETFD));
// 		// 	printf("pipefd[1] : %d\n", fcntl(pipefd[1], F_GETFD));
// 		// }
// 		execve(path, arg, env);
// 		free(path);
// 		free_array(arg);
// 		printf("%s: command not found\n", arg[0]); //print temporaire
// 		exit(127);
// 	}
// 	printf("free de path et arg\n");
// 	free(path);
// 	free_array(arg);
// 	return (pid);
// }

// pid_t	exec_node_ast(t_ast *ast, t_cmd *cmd, int in, int out, char **env, int pipefd[2], int **to_close)
// {
// 	int			status;
// 	pid_t		pid;
// 	pid_t		pid2;
// 	t_in_out	files;
// 	extern int exit_code;

// 	printf("-------------exec node ast-------------\n");
// 	// sleep(1);
// 	status = 0;
// 	pid = 0;
// 	printf("check du noeud\n");
// 	if (!ast)
// 	{
// 		printf("pas de noeud\n");
// 		return (0);
// 	}
// 	printf("check de la commande\n");
// 	if (!cmd)
// 	{
// 		printf("malloc de cmd\n");
// 		cmd = malloc(sizeof(t_cmd));
// 		if (!cmd)
// 			return (printf("probleme de malloc\n")); //print temp
// 		cmd->content = NULL;
// 		cmd->fd_in = -1;
// 		cmd->fd_out = -1;
// 	}
// 	files.infile = -1;
// 	files.outfile = -1;
// 	printf("cmd existe\n");
// 	if (cmd && cmd->content)
// 		printf("cmd->content = %s\n", cmd->content);
// 	printf("test\n");
// 	printf("cmd->fd_in = %d\n", cmd->fd_in);
// 	printf("test\n");
// 	printf("cmd->fd_out = %d\n", cmd->fd_out);
// 	printf("test\n");




// 	if (ast->type == PIPE)
// 	{
// 		printf("on trouve un pipe\n");
// 		int	newpipefd[2];
	
// 		if (pipe(newpipefd) == -1)
// 			return (printf("probleme de pipe\n"));//temp print
// 		printf("creation du pipe\n");
// 		if (!add_pipe_to_close(newpipefd, to_close))
// 			return (printf("probleme de pipe\n"));//temp print
// 		if (to_close)
// 			printf("to_close existe\n");
// 		else
// 			printf("to_close n'existe pas \n");
// 		sleep(1);
// 		if (ast->left)
// 		{
// 			printf("on envoie ast->left a gauche depuis pipe\n");
// 			printf("ast->left est de type = %d\n", ast->left->type);

// 			if (pipefd)
// 				cmd->fd_in = pipefd[0];
// 			cmd->fd_out = newpipefd[1];
// 			pid = exec_node_ast(ast->left, cmd, in, out, env, newpipefd, to_close);
// 			cmd->fd_in = -1;
// 			cmd->fd_out = -1;

// 			printf("revenu\n");
// 			printf("-------------revenu dans la fonction du pipe-------------\n");
// 		}
// 		if (ast->right)
// 		{
// 			printf("on envoie ast->right a droite depuis pipe\n");
// 			printf("ast->right est de type = %d\n", ast->right->type);

// 			if (pipefd)
// 				cmd->fd_out = pipefd[1];
// 			cmd->fd_in = newpipefd[0];
// 			pid2 = exec_node_ast(ast->right, cmd, in, out, env, newpipefd, to_close);
// 			cmd->fd_in = -1;
// 			cmd->fd_out = -1;

				
// 			printf("-------------revenu dans la fonction du pipe-------------\n");
// 		}
// 		printf("on ferme les pipe[1] et pipe[0]\n");
// 		close_all_pipes(to_close);
// 		// close(newpipefd[0]);
// 		// close(newpipefd[1]);
// 		// if (pipefd)
// 		// {
// 		// 	close(pipefd[0]);
// 		// 	close(pipefd[1]);
// 		// }
// 		printf("newpipe ferme\n");
// 		waitpid(pid, &status, 0);
// 		printf("pid1 fini\n");
// 		waitpid(pid2, &status, 0);
// 		printf("pid2 finit\n");
// 		exit_code = WEXITSTATUS(status);
// 	}




// 	else if (ast->type >= R_INPUT && ast->type != R_HEREDOC)
// 	{
// 		printf("on trouve une redir\n");
// 		if (ast->right && ast->right->type >= R_INPUT)
// 		{
// 			printf("on trouve deux redir cote a cote\n");
// 			t_ast	*current;
		
// 			current = ast;
// 			files = change_redir(current, cmd);
// 			if (files.infile == -2 || files.outfile == -2)
// 			{
// 				printf("les redirections ont foires\n");
// 				if (cmd && cmd->content)
// 			}
// 			current->done = 1;
// 			while (current->right && current->right->type >= R_INPUT)
// 			{
// 				current = current->right;
// 				files = change_redir(current, cmd);
// 				if (files.infile == -2 || files.outfile == -2)
// 				{
// 					printf("les redirections ont foires\n");
// 					if (cmd && cmd->content)
// 						free(cmd);
// 					if (cmd)
// 						free(cmd);
// 					return (1);
// 				}
// 				current->done = 1;
// 				printf("on avance current\n");
// 			}
// 		}
// 		else
// 		{
// 			files = change_redir(ast, cmd);
// 			if (files.infile == -2 || files.outfile == -2)
// 			{
// 				printf("les redirections ont foires\n");
// 				if (cmd && cmd->content)
// 					free(cmd);
// 				if (cmd)
// 					free(cmd);
// 				return (1);
// 			}
// 		}
// 		ast->done = 1;
// 		if (ast->left && !ast->left->done)
// 		{
// 			printf("on envoie ast->left a gauche depuis redir\n");
// 			printf("ast->left est de type = %d\n", ast->left->type);


// 			exec_node_ast(ast->left, cmd, files.infile, files.outfile, env, pipefd, to_close);


// 			printf("-------------revenu dans la fonction de la redir-------------\n");
// 		}
// 		if (ast->right && !ast->right->done)
// 		{
// 			printf("on envoie ast->right a droite depuis redir\n");
// 			printf("ast->right est de type = %d\n", ast->right->type);


// 			exec_node_ast(ast->right, cmd, files.infile, files.outfile, env, pipefd, to_close);

			
// 			printf("-------------revenu dans la fonction de la redir-------------\n");
// 		}
// 	}



// 	else if (ast->type == WORD)
// 	{
// 		printf("strdup de %s\n", ast->content);
// 		if (cmd->content)
// 			free(cmd->content);
// 		cmd->content = ft_strdup(ast->content);
// 		if (!cmd->content)
// 			return (printf("probleme de malloc\n")); //print temp
// 		pid = exec_cmd(cmd, env, pipefd, in, out, to_close);
// 		if (pid == -1)
// 		{
// 			printf("probleme de fork, on free\n");
// 			free(cmd->content);
// 			free(cmd);
// 			return (0);
// 		}
// 		printf("pid wait = %d\n", pid);
// 		if (in != -1)
// 			close(in);
// 		if (out != -1)
// 			close(out);
// 		if (!pipefd)
// 		{
// 			waitpid(pid, &status, 0);
// 			printf("on a fini d'attendre\n");
// 			exit_code = WEXITSTATUS(status);
// 		}
// 		printf("cmd->fdin : %d\ncmd->fdout : %d\n", cmd->fd_in, cmd->fd_out);
// 		cmd->fd_in = -1;
// 		cmd->fd_out = -1;
// 		printf("tout est free et close\n");
// 	}


// 			}
// 			current->done = 1;
// 			while (current->right && current->right->type >= R_INPUT)
// 			{
// 				current = current->right;
// 				files = change_redir(current, cmd);
// 				if (files.infile == -2 || files.outfile == -2)
// 				{
// 					printf("les redirections ont foires\n");
// 					if (cmd && cmd->content)
// 						free(cmd);
// 					if (cmd)
// 						free(cmd);
// 					return (1);
// 				}
// 				current->done = 1;
// 				printf("on avance current\n");
// 			}
// 		}
// 		else
// 		{
// 			files = change_redir(ast, cmd);
// 			if (files.infile == -2 || files.outfile == -2)
// 			{
// 				printf("les redirections ont foires\n");
// 				if (cmd && cmd->content)
// 					free(cmd);
// 				if (cmd)
// 					free(cmd);
// 				return (1);
// 			}
// 		}
// 		ast->done = 1;
// 		if (ast->left && !ast->left->done)
// 		{
// 			printf("on envoie ast->left a gauche depuis redir\n");
// 			printf("ast->left est de type = %d\n", ast->left->type);


// 			exec_node_ast(ast->left, cmd, files.infile, files.outfile, env, pipefd, to_close);


// 			printf("-------------revenu dans la fonction de la redir-------------\n");
// 		}
// 		if (ast->right && !ast->right->done)
// 		{
// 			printf("on envoie ast->right a droite depuis redir\n");
// 			printf("ast->right est de type = %d\n", ast->right->type);


// 			exec_node_ast(ast->right, cmd, files.infile, files.outfile, env, pipefd, to_close);

			
// 			printf("-------------revenu dans la fonction de la redir-------------\n");
// 		}
// 	}



// 	else if (ast->type == WORD)
// 	{
// 		printf("strdup de %s\n", ast->content);
// 		if (cmd->content)
// 			free(cmd->content);
// 		cmd->content = ft_strdup(ast->content);
// 		if (!cmd->content)
// 			return (printf("probleme de malloc\n")); //print temp
// 		pid = exec_cmd(cmd, env, pipefd, in, out, to_close);
// 		if (pid == -1)
// 		{
// 			printf("probleme de fork, on free\n");
// 			free(cmd->content);
// 			free(cmd);
// 			return (0);
// 		}
// 		printf("pid wait = %d\n", pid);
// 		if (in != -1)
// 			close(in);
// 		if (out != -1)
// 			close(out);
// 		if (!pipefd)
// 		{
// 			waitpid(pid, &status, 0);
// 			printf("on a fini d'attendre\n");
// 			exit_code = WEXITSTATUS(status);
// 		}
// 		printf("cmd->fdin : %d\ncmd->fdout : %d\n", cmd->fd_in, cmd->fd_out);
// 		cmd->fd_in = -1;
// 		cmd->fd_out = -1;
// 		printf("tout est free et close\n");
// 	}




// 	else
// 	{
// 		printf("on free tout\n");
// 		if (cmd && cmd->content)
// 		{
// 			printf("cmd->content = %s\n", cmd->content);
// 			free(cmd->content);
// 		}
// 		if (cmd)
// 			free(cmd);
// 	}
// 	printf("fin de exec_node_ast\n");
// 	printf("ce qu'on retourne : %d\n", WEXITSTATUS(status));
// 	return (pid);
// }


// 	else
// 	{
// 		printf("on free tout\n");
// 		if (cmd && cmd->content)
// 		{
// 			printf("cmd->content = %s\n", cmd->content);
// 			free(cmd->content);
// 		}
// 		if (cmd)
// 			free(cmd);
// 	}
// 	printf("fin de exec_node_ast\n");
// 	printf("ce qu'on retourne : %d\n", WEXITSTATUS(status));
// 	return (pid);
// }
// 					free(cmd);
// 				if (cmd)
// 					free(cmd);
// 				return (1);
// 			}
// 			current->done = 1;
// 			while (current->right && current->right->type >= R_INPUT)
// 			{
// 				current = current->right;
// 				files = change_redir(current, cmd);
// 				if (files.infile == -2 || files.outfile == -2)
// 				{
// 					printf("les redirections ont foires\n");
// 					if (cmd && cmd->content)
// 						free(cmd);
// 					if (cmd)
// 						free(cmd);
// 					return (1);
// 				}
// 				current->done = 1;
// 				printf("on avance current\n");
// 			}
// 		}
// 		else
// 		{
// 			files = change_redir(ast, cmd);
// 			if (files.infile == -2 || files.outfile == -2)
// 			{
// 				printf("les redirections ont foires\n");
// 				if (cmd && cmd->content)
// 					free(cmd);
// 				if (cmd)
// 					free(cmd);
// 				return (1);
// 			}
// 		}
// 		ast->done = 1;
// 		if (ast->left && !ast->left->done)
// 		{
// 			printf("on envoie ast->left a gauche depuis redir\n");
// 			printf("ast->left est de type = %d\n", ast->left->type);


// 			exec_node_ast(ast->left, cmd, files.infile, files.outfile, env, pipefd, to_close);


// 			printf("-------------revenu dans la fonction de la redir-------------\n");
// 		}
// 		if (ast->right && !ast->right->done)
// 		{
// 			printf("on envoie ast->right a droite depuis redir\n");
// 			printf("ast->right est de type = %d\n", ast->right->type);


// 			exec_node_ast(ast->right, cmd, files.infile, files.outfile, env, pipefd, to_close);

			
// 			printf("-------------revenu dans la fonction de la redir-------------\n");
// 		}
// 	}



// 	else if (ast->type == WORD)
// 	{
// 		printf("strdup de %s\n", ast->content);
// 		if (cmd->content)
// 			free(cmd->content);
// 		cmd->content = ft_strdup(ast->content);
// 		if (!cmd->content)
// 			return (printf("probleme de malloc\n")); //print temp
// 		pid = exec_cmd(cmd, env, pipefd, in, out, to_close);
// 		if (pid == -1)
// 		{
// 			printf("probleme de fork, on free\n");
// 			free(cmd->content);
// 			free(cmd);
// 			return (0);
// 		}
// 		printf("pid wait = %d\n", pid);
// 		if (in != -1)
// 			close(in);
// 		if (out != -1)
// 			close(out);
// 		if (!pipefd)
// 		{
// 			waitpid(pid, &status, 0);
// 			printf("on a fini d'attendre\n");
// 			exit_code = WEXITSTATUS(status);
// 		}
// 		printf("cmd->fdin : %d\ncmd->fdout : %d\n", cmd->fd_in, cmd->fd_out);
// 		cmd->fd_in = -1;
// 		cmd->fd_out = -1;
// 		printf("tout est free et close\n");
// 	}




// 	else
// 	{
// 		printf("on free tout\n");
// 		if (cmd && cmd->content)
// 		{
// 			printf("cmd->content = %s\n", cmd->content);
// 			free(cmd->content);
// 		}
// 		if (cmd)
// 			free(cmd);
// 	}
// 	printf("fin de exec_node_ast\n");
// 	printf("ce qu'on retourne : %d\n", WEXITSTATUS(status));
// 	return (pid);
// }

t_inout	return_wrong_fds(t_ast *ast)
{
	t_inout	fd;

	print_open_error(ast->content);
	fd.infile = -2;
	fd.outfile = -2;
	return (fd);
}

t_inout	change_redir(t_ast *ast)
{
	t_inout fd;

	fd.infile = -1;
	fd.outfile = -1;
	if (ast->type == R_INPUT)
	{
		// printf("input trouve\n");
		// printf("open de %s\n", ast->content);
		fd.infile = open(ast->content, O_RDONLY, 0777);
		if (fd.infile == -1)
			return (return_wrong_fds(ast));
		// {
		// 	print_open_error(ast->content);
		// 	fd.infile = -2;
		// 	fd.outfile = -2;
		// 	return (fd);
		// }
		// printf("ouverture reussie\n");
		// if (cmd->fd_in != -1)
		// {
		// 	printf("fd_in != -1 donc on ferme d'abord\n");
		// 	close(cmd->fd_in);
		// }
		// cmd->fd_in = fd.infile;
		// printf("on assigne %d a fd_in\n", fd.infile);
		// sleep(1);
	}
	else if (ast->type == R_TRUNC || ast->type == R_TRUNC_NOCLOBBER)
	{
		// printf("trunc trouve\n");
		// printf("open de %s\n", ast->content);
		fd.outfile = open(ast->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd.outfile == -1)
			return (return_wrong_fds(ast));
		// {
		// 	print_open_error(ast->content);
		// 	fd.infile = -2;
		// 	fd.outfile = -2;
		// 	return (fd);
		// }
		// printf("ouverture reussie\n");
		// if (cmd->fd_out != -1)
		// {
		// 	printf("fd_out != -1 donc on ferme\n");
		// 	close(cmd->fd_out);
		// }
		// printf("on assigne %d\n", fd.outfile);
		// cmd->fd_out = fd.outfile;
	}
	else if (ast->type == R_APPEND)
	{
		// printf("append trouve\n");
		// printf("open de %s\n", ast->content);
		fd.outfile = open(ast->content, O_WRONLY | O_CREAT |O_APPEND, 0644);
		if (fd.outfile == -1)
			return (return_wrong_fds(ast));
		// {
		// 	print_open_error(ast->content);
		// 	fd.infile = -2;
		// 	fd.outfile = -2;
		// 	return (fd);
		// }
		// printf("ouverture reussie\n");
		// if (cmd->fd_out != -1)
		// {
		// 	printf("fd_out != -1 donc on ferme\n");
		// 	close(cmd->fd_out);
		// }
		// printf("on assigne %d\n", file.outfile);
		// cmd->fd_out = file.outfile;
	}
	return (fd);
}

t_inout	get_fd(t_ast *ast)
{
	t_inout	fd;

	fd.infile = -1;
	fd.outfile = -1;
	while (ast)
	{
		// printf("%sast->done = : %s%d%s\n", GREEN, BYELLOW, ast->done, END);
		// if (ast->left)
			// printf("%sast->left->done = : %s%d%s\n", GREEN, BYELLOW, ast->left->done, END);
		// if (ast->right)
			// printf("%sast->right->done = : %s%d%s\n", GREEN, BYELLOW, ast->right->done, END);
		// sleep(1);
		if (fd.infile != -1)
			close(fd.infile);
		if (fd.outfile != -1)
			close(fd.outfile);
		fd = change_redir(ast);
		if (fd.infile == -2 || fd.outfile == -2)
			return (fd);
		ast->done = 1;
		if (ast->left && ast->left->type >= R_INPUT && ast->left->done == 0)
		{
			// printf("fd left\n");
			ast = ast->left;
		}
		else if (ast->right && ast->right->type >= R_INPUT && ast->right->done == 0)
		{
			// printf("fd right\n");
			ast = ast->right;
		}
		else if (ast->top && ast->top->type != PIPE)
		{
			// printf("fd top\n");
			ast = ast->top;
		}
		else
		{
			// printf("fd break\n");
			break ;
		}
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
	// printf("%s\nCREATE_CMDS_ARRAY%s\n", BRED, END);
	while (current && i < cmd[0].num_of_cmds)
	{
		fd.infile = -1;
		fd.outfile = -1;
		// printf("%scurrent->done = : %s%d%s\n", GREEN, BYELLOW, current->done, END);
		// printf("%son entre dans la boucle%s\n", CYAN, END);
		// sleep(1);
		if (current->type == PIPE)
		{
			// printf("%scurrent->type est un PIPE%s\n", PURPLE, END);
			while (current->type == PIPE)
			{
				if (current->left && (current->left->done == 0 || current->left->type == PIPE))
					current = current->left;
				else if (current->right && (current->right->done == 0 || current->right->type == PIPE))
					current = current->right;
			}
		}
		if (current->type >= R_INPUT && current->type != R_HEREDOC && current->done == 0)
		{
			// printf("%scurrent->type est une redir%s\n", PURPLE, END);
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
			// printf("%scurrent->type est un word%s\n", PURPLE, END);
			current->done = 1;
			cmd->cmd[i].content = ft_strdup(current->content);
			if (!cmd->cmd[i].content)
				return (0);
			cmd->cmd[i].fd_in = fd.infile;
			cmd->cmd[i].fd_out = fd.outfile;
		}
		if (cmd->cmd[i].content)
			i++;
		current = ast;
		// sleep(1);
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
		i++;
	}
	free(cmd->cmd);
	cmd->cmd = NULL;
}

t_cmd_info	get_cmd_array(t_ast *ast)
{
	t_cmd_info	cmd;

	// printf("%sGET_CMD_ARRAY%s\n", BRED, END);
	cmd.num_of_cmds = get_cmd_number(ast);
	// printf("%scommand number : %s%d%s\n", GREEN, BYELLOW, cmd.num_of_cmds, END);
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
		printf("%s\tcontent -- %s%s\n", YELLOW, cmd.cmd[i].content, END);
		printf("%s\tfd_in   -- %d%s\n", YELLOW, cmd.cmd[i].fd_in, END);
		printf("%s\tfd_out  -- %d%s\n", YELLOW, cmd.cmd[i].fd_out, END);
		i++;
	}
}

void	exec_cmds(t_cmd_info cmd, char **env)
{
	char		*path;
	char		**arg;
	pid_t		*pid;
	int			i_pid;
	int			i;
	int			newpipefd[2];
	int			oldpipefd;
	int			status;
	extern int	exit_code;

	i = 0;
	i_pid = 0;
	status = 0;
	newpipefd[0] = -1;
	newpipefd[1] = -1;
	oldpipefd = -1;
	pid = malloc(cmd.num_of_cmds);
	if (!pid)
	{
		// printf("probleme pid\n");
		print_error_message(1, "malloc", "Cannot allocate memory");
		return ;
	}
	// oldpipefd[0] = -1;
	// oldpipefd[1] = -1;
	// fprintf(stderr, "%sGREEN pour le child\n%sBLUE pour le parent\n\n%s", GREEN, BLUE, END);
	while (i < cmd.num_of_cmds)
	{
		path = right_path(cmd.cmd[i].content, env);
		if (!path)
		{
			// printf("probleme path\n");//temp
			print_error_message(1, "malloc", "Cannot allocate memory");
			return ;
		}
		arg = ft_split(cmd.cmd[i].content, ' ');
		if (!arg)
		{
			free(path);
			print_error_message(1, "malloc", "Cannot allocate memory");
			// printf("probleme arg\n");//temp
			return ;
		}
		if (i < cmd.num_of_cmds - 1 && pipe(newpipefd) == -1) //line 1039
		{
			free(path);
			free_array(arg);
			print_error_message(1, "pipe", "Cannot allocate memory");
			// printf("probleme pipe\n");
			return ;
		}
		pid[i_pid] = fork();
		if (pid[i_pid] == -1)
		{
			free(path);
			free_array(arg);
			print_error_message(1, "fork", "Cannot allocate memory");
			// printf("probleme fork\n");//temp
			return ;
		}
		if (!pid[i_pid])
		{
			if (cmd.num_of_cmds > 1)
			{
				if (i == 0 && cmd.cmd[i].fd_out == -1)
				{
					// fprintf(stderr, "%sfd out devient newpipefd[1] (ecriture) parce que commencement\n%s", GREEN, END);
					cmd.cmd[i].fd_out = newpipefd[1];
				}
				else if (i == cmd.num_of_cmds - 1 && cmd.cmd[i].fd_in == -1)
				{
					// fprintf(stderr, "%sfd in devient oldpipefd[0] (lecture) parce que fin\n%s", GREEN, END);
					cmd.cmd[i].fd_in = oldpipefd;
				}
				else if (i > 0 && i < cmd.num_of_cmds - 1)
				{
					if (cmd.cmd[i].fd_out == -1)
					{
						// fprintf(stderr, "%sfd out devient newpipefd[1] (ecriture)\n%s", GREEN, END);
						cmd.cmd[i].fd_out = newpipefd[1];
					}
					if (cmd.cmd[i].fd_in == -1)
					{
						// fprintf(stderr, "%sfd in devient oldpipefd[0] (lecture)\n%s", GREEN, END);
						cmd.cmd[i].fd_in = oldpipefd;
					}
				}
			}
			if (cmd.cmd[i].fd_in != -1)
			{
				// fprintf(stderr, "%sdup2 de fdin a stdin fileno\n%s", GREEN, END);
				dup2(cmd.cmd[i].fd_in, STDIN_FILENO);
				close(cmd.cmd[i].fd_in);
			}
			if (cmd.cmd[i].fd_out != -1)
			{
				// fprintf(stderr, "%sdup2 de fdout a stdout fileno\n%s", GREEN, END);
				dup2(cmd.cmd[i].fd_out, STDOUT_FILENO);
				close(cmd.cmd[i].fd_out);
			}
			if (newpipefd[0] != -1)
			{
				// fprintf(stderr, "%sclose de newpipe[0]\n%s", GREEN, END);
				close(newpipefd[0]);
			}
			if (newpipefd[1] != -1)
			{
				// fprintf(stderr, "%sclose de newpipe[1]\n%s", GREEN, END);
				close(newpipefd[1]);
			}
			if (oldpipefd != -1)
			{
				// fprintf(stderr, "%sclose de oldpipe[0]\n%s", GREEN, END);
				close(oldpipefd);
			}
			execve(path, arg, env);
			print_error_message(1, arg[0], "command not found");
			// fprintf(stderr, "%s: command not found\n", arg[0]); //print temporaire
			free(path);
			free_array(arg);
			exit(127);
		}
		// fprintf(stderr, "%son free\n%s", BLUE, END);
		free(path);
		free_array(arg);

		if (newpipefd[1] != -1)
		{
			// fprintf(stderr, "%sclose de newpipefd[1]\n%s", BLUE, END);
			close(newpipefd[1]);
		}

		if (oldpipefd != -1)
		{
			// fprintf(stderr, "%sclose de oldpipefd[0]\n%s", BLUE, END);
			close(oldpipefd);
		}

		oldpipefd = dup(newpipefd[0]);
		// fprintf(stderr, "%soldpipefd[0] = newpipefd[0]\n%s", BLUE, END);
		close(newpipefd[0]);
		newpipefd[0] = -1;
		exit_code = WEXITSTATUS(status);
		i++;
		i_pid++;
	}
	if (oldpipefd != -1)
		close(oldpipefd);
	int j = 0;
	// fprintf(stderr, "i_pid = %d\n", i_pid);
	while (j < i_pid)
	{
		// fprintf(stderr, "on attend %d\n", pid[j]);
		waitpid(pid[j], &status, 0);
		j++;
	}
	exit_code = WEXITSTATUS(status);
	// printf("exit_code : %d\n", exit_code);
}

int	exec(t_ast *ast, char **env, int m)
{
	int		status;
	t_cmd_info	cmd;

	(void)m;
	(void)env;
	status = 0;
	cmd = get_cmd_array(ast);
	if (!cmd.cmd || !cmd.cmd->content)
		return (0);//temp print
	// display_cmds(cmd);
	exec_cmds(cmd, env);
	free_cmd(&cmd);
	// printf("on entre dans exec_node_ast\n");
	// status = exec_node_ast(ast, NULL, -1, -1, env, NULL, NULL);
	return (status);
}

