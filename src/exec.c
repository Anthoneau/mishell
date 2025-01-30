/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:35:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/30 16:51:55 by agoldber         ###   ########.fr       */
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

t_in_out	change_redir(t_ast *ast, t_cmd *cmd)
{
	t_in_out file;

	file.infile = -1;
	file.outfile = -1;
	printf("-------------change redir-------------\n");
	if (ast->type == R_INPUT)
	{
		printf("input trouve\n");
		printf("open de %s\n", ast->content);
		file.infile = open(ast->content, O_RDONLY, 0777);
		if (file.infile == -1)
		{
			print_open_error(ast->content);
			file.infile = -2;
			file.outfile = -2;
			return (file);
		}
		printf("ouverture reussie\n");
		if (cmd->fd_in != -1)
		{
			printf("fd_in != -1 donc on ferme d'abord\n");
			close(cmd->fd_in);
		}
		cmd->fd_in = file.infile;
		printf("on assigne %d a fd_in\n", file.infile);
		// sleep(1);
	}
	else if (ast->type == R_TRUNC || ast->type == R_TRUNC_NOCLOBBER)
	{
		printf("trunc trouve\n");
		printf("open de %s\n", ast->content);
		file.outfile = open(ast->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file.outfile == -1)
		{
			print_open_error(ast->content);
			file.infile = -2;
			file.outfile = -2;
			return (file);
		}
		printf("ouverture reussie\n");
		if (cmd->fd_out != -1)
		{
			printf("fd_out != -1 donc on ferme\n");
			close(cmd->fd_out);
		}
		printf("on assigne %d\n", file.outfile);
		cmd->fd_out = file.outfile;
	}
	else if (ast->type == R_APPEND)
	{
		printf("append trouve\n");
		printf("open de %s\n", ast->content);
		file.outfile = open(ast->content, O_WRONLY | O_CREAT |O_APPEND, 0644);
		if (file.outfile == -1)
		{
			print_open_error(ast->content);
			file.infile = -2;
			file.outfile = -2;
			return (file);
		}
		printf("ouverture reussie\n");
		if (cmd->fd_out != -1)
		{
			printf("fd_out != -1 donc on ferme\n");
			close(cmd->fd_out);
		}
		printf("on assigne %d\n", file.outfile);
		cmd->fd_out = file.outfile;
	}
	return (file);
}

pid_t	exec_cmd(t_cmd *cmd, char **env, int pipefd[2], int in, int out)
{
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
		if (in != -1)
		{
			dup2(in, STDIN_FILENO);
			close(in);
			if (cmd->fd_in != -1)
				close(cmd->fd_in);
		}
		else if (cmd->fd_in != -1)
		{
			dup2(cmd->fd_in, STDIN_FILENO);
			close(cmd->fd_in);
		}
		if (out != -1)
		{
			dup2(out, STDOUT_FILENO);
			close(in);
			if (cmd->fd_out != -1)
				close(cmd->fd_out);
		}
		else if (cmd->fd_out != -1)
		{
			dup2(cmd->fd_out, STDOUT_FILENO);
			close(cmd->fd_out);
		}
		if (pipefd)
		{
			close(pipefd[0]);
			close(pipefd[1]);
		}
		printf("in : %d\n", fcntl(in, F_GETFD));
		printf("out : %d\n", fcntl(out, F_GETFD));
		printf("cmd->fd_in : %d\n", fcntl(cmd->fd_in, F_GETFD));
		printf("cmd->fd_out : %d\n", fcntl(cmd->fd_out, F_GETFD));
		printf("pipefd[0] : %d\n", fcntl(pipefd[0], F_GETFD));
		printf("pipefd[1] : %d\n", fcntl(pipefd[1], F_GETFD));
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

pid_t	exec_node_ast(t_ast *ast, t_cmd *cmd, int in, int out, char **env, int pipefd[2])
{
	int			status;
	pid_t		pid;
	pid_t		pid2;
	t_in_out	files;
	extern int exit_code;

	printf("-------------exec node ast-------------\n");
	// sleep(1);
	status = 0;
	pid = 0;
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
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
			return (printf("probleme de malloc\n")); //print temp
		cmd->content = NULL;
		cmd->fd_in = -1;
		cmd->fd_out = -1;
	}
	files.infile = -1;
	files.outfile = -1;
	printf("cmd existe\n");
	if (cmd && cmd->content)
		printf("cmd->content = %s\n", cmd->content);
	printf("test\n");
	printf("cmd->fd_in = %d\n", cmd->fd_in);
	printf("test\n");
	printf("cmd->fd_out = %d\n", cmd->fd_out);
	printf("test\n");




	if (ast->type == PIPE)
	{
		printf("on trouve un pipe\n");
		int	newpipefd[2];
	
		if (pipe(newpipefd) == -1)
			return (printf("probleme de pipe\n"));
		printf("creation du pipe\n");
		if (ast->left)
		{
			printf("on envoie ast->left a gauche depuis pipe\n");
			printf("ast->left est de type = %d\n", ast->left->type);

			if (pipefd)
				cmd->fd_in = pipefd[0];
			cmd->fd_out = newpipefd[1];
			pid = exec_node_ast(ast->left, cmd, in, out, env, newpipefd);
			cmd->fd_in = -1;
			cmd->fd_out = -1;

			printf("revenu\n");
			printf("-------------revenu dans la fonction du pipe-------------\n");
		}
		if (ast->right)
		{
			printf("on envoie ast->right a droite depuis pipe\n");
			printf("ast->right est de type = %d\n", ast->right->type);

			if (pipefd)
				cmd->fd_out = pipefd[1];
			cmd->fd_in = newpipefd[0];
			pid2 = exec_node_ast(ast->right, cmd, in, out, env, newpipefd);
			cmd->fd_in = -1;
			cmd->fd_out = -1;
			
			//if (cmd && cmd->content)
			//	free(cmd->content);
			//if (cmd)
			//	free(cmd);

				
			printf("-------------revenu dans la fonction du pipe-------------\n");
		}
		printf("on ferme les pipe[1] et pipe[0]\n");
		// if (pipefd)
		// {
		// 	close(pipefd[0]);
		// 	close(pipefd[1]);
		// }
		close(newpipefd[0]);
		close(newpipefd[1]);
		printf("newpipe ferme\n");
		waitpid(pid, &status, 0);
		printf("pid1 fini\n");
		waitpid(pid2, &status, 0);
		printf("pid2 finit\n");
		exit_code = WEXITSTATUS(status);
	}




	else if (ast->type >= R_INPUT && ast->type != R_HEREDOC)
	{
		printf("on trouve une redir\n");
		if (ast->right && ast->right->type >= R_INPUT)
		{
			printf("on trouve deux redir cote a cote\n");
			t_ast	*current;
		
			current = ast;
			files = change_redir(current, cmd);
			if (files.infile == -2 || files.outfile == -2)
			{
				printf("les redirections ont foires\n");
				if (cmd && cmd->content)
					free(cmd);
				if (cmd)
					free(cmd);
				return (1);
			}
			current->done = 1;
			while (current->right && current->right->type >= R_INPUT)
			{
				current = current->right;
				files = change_redir(current, cmd);
				if (files.infile == -2 || files.outfile == -2)
				{
					printf("les redirections ont foires\n");
					if (cmd && cmd->content)
						free(cmd);
					if (cmd)
						free(cmd);
					return (1);
				}
				current->done = 1;
				printf("on avance current\n");
			}
		}
		else
		{
			files = change_redir(ast, cmd);
			if (files.infile == -2 || files.outfile == -2)
			{
				printf("les redirections ont foires\n");
				if (cmd && cmd->content)
					free(cmd);
				if (cmd)
					free(cmd);
				return (1);
			}
		}
		ast->done = 1;
		if (ast->left && !ast->left->done)
		{
			printf("on envoie ast->left a gauche depuis redir\n");
			printf("ast->left est de type = %d\n", ast->left->type);


			exec_node_ast(ast->left, cmd, files.infile, files.outfile, env, pipefd);


			printf("-------------revenu dans la fonction de la redir-------------\n");
		}
		if (ast->right && !ast->right->done)
		{
			printf("on envoie ast->right a droite depuis redir\n");
			printf("ast->right est de type = %d\n", ast->right->type);


			exec_node_ast(ast->right, cmd, files.infile, files.outfile, env, pipefd);

			
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
		// if (in != -1 && cmd->fd_in == -1)
		// {
		// 	printf("in = %d et cmd->fd_in == -1\n", in);
		// 	cmd->fd_in = in;
		// }
		// if (out != -1 && cmd->fd_out == -1)
		// {
		// 	printf("out = %d et cmd->fd_out == -1\n", out);
		// 	cmd->fd_out = out;
		// }
		pid = exec_cmd(cmd, env, pipefd, in, out);
		if (pid == -1)
		{
			printf("probleme de fork, on free\n");
			free(cmd->content);
			free(cmd);
			return (0);
		}
		printf("pid wait = %d\n", pid);
		if (in != -1)
			close(in);
		if (out != -1)
			close(out);
		// if (cmd->fd_in != -1)
		// 	close(cmd->fd_in);
		// if (cmd->fd_out != -1)
		// 	close(cmd->fd_out);
		// if (in != -1)
		// 	close(in);
		// if (out != -1)
		// 	close(out);
		// if (pipefd && pipefd[0])
		// 	close(pipefd[0]);
		// if (pipefd && pipefd[1])
		// 	close(pipefd[1]);
		// waitpid(pid, &status, 0);
		printf("on a fini d'attendre\n");
		printf("cmd->fdin : %d\ncmd->fdout : %d\n", cmd->fd_in, cmd->fd_out);
		cmd->fd_in = -1;
		cmd->fd_out = -1;
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
	return (pid);
}

//int	tot_num_cmd(t_ast *ast)
//{
//	int		number_of_commands;

//	number_of_commands = 0;
//	if (!ast)
//		return (0);
//	if (ast->type == WORD)
//		number_of_commands++;
//	if (ast->right)
//		number_of_commands += tot_num_cmd(ast->right);
//	if (ast->left)
//		number_of_commands += tot_num_cmd(ast->left);
//	return (number_of_commands);
//}

//void	exec_node_ast(t_ast *ast)
//{
//	int		pipefd[2];
//	int		oldpipe[2];
//	int		cmd_num;
//	int		n;
//	t_cmd	cmd;
//	pid_t	pid;

//	cmd_num = 0;
//	cmd.fd_in = STDIN_FILENO;
//	cmd.fd_out = STDOUT_FILENO;
//	n = tot_num_cmd(ast);
	
	//while (1)
	//{
	//	if (pipe(pipefd) == -1)
	//		return ; //free
		
		//if (cmd_num == 0)
		//{
		//	close(pipefd[0]);
		//}
		//else if (cmd_num = n - 1)
		//{
		//	close(pipefd[1]);
		//}
		//else
		//{
			
		//}

		//char	*path;
		//char	**arg;
		//pid_t	pid;

		//path = right_path(cmd->content, env);
		//if (!path)
		//	return ;
		//arg = ft_split(cmd->content, ' ');
		//if (!arg)
		//{
		//	free(path);//print temp
		//	return ;
		//}
		//pid = fork();
		//if (pid == -1)
		//{
		//	free(path);
		//	free_array(arg);//print temporaire
		//	return ;
		//}
		//if (!pid)
		//{
		//	execve(path, arg, env);
		//	free(path);
		//	free_array(arg);
		//	printf("%s: command not found\n", arg[0]); //print temporaire
		//	exit(127);
		//}
		//free(path);
		//free_array(arg);
		//}
//}

int	exec(t_ast *ast, char **env, int m)
{
	int		status;

	(void)m;
	status = 0;
	printf("on entre dans exec_node_ast\n");
	status = exec_node_ast(ast, NULL, -1, -1, env, NULL);
	return (status);
}
