/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:14:09 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/24 16:14:15 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	find_current_pipe(t_ast **current)
{
	while ((*current)->type == PIPE)
	{
		if ((*current)->left && ((*current)->left->done == 0 || (*current)->left->type == PIPE))
			(*current) = (*current)->left;
		else if ((*current)->right && ((*current)->right->done == 0 || (*current)->right->type == PIPE))
			(*current) = (*current)->right;
	}
}

int	get_arg_and_fd(t_ast **current, t_cmd_info **cmd, t_inout fd, int i)
{
	(*current)->done = 1;
	(*cmd)->cmd[i].arg = ft_arrdup((*current)->arg);
	if (!(*cmd)->cmd[i].arg)
		return (0);
	(*cmd)->cmd[i].fd_in = fd.infile;
	(*cmd)->cmd[i].fd_out = fd.outfile;
	return (1);
}

int	get_cmds_inputs(t_ast **current, t_inout *fd)
{
	*fd = get_fd(*current);
	if (fd->infile == -2 || fd->outfile == -2)
		return (0);
	(*current)->done = 1;
	(*current) = find_new_current(*current);
	if (!*current)
		return (0);
	return (1);
}

int	create_cmds_array(t_cmd_info *cmd, t_ast *ast)
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
			find_current_pipe(&current);
		if (current->type >= R_INPUT && current->done == 0
			&& !get_cmds_inputs(&current, &fd))
			return (0);
		if (current->type == WORD && current->done == 0
			&& !get_arg_and_fd(&current, &cmd, fd, i))
			return (0);
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
