/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:31:18 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/17 13:47:01 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else if (ast->type == R_TRUNC || ast->type == R_TRUNC_NOCLOBBER
		|| ast->type == R_APPEND)
	{
		if (fd->outfile != -1)
			close(fd->outfile);
		if (ast->type == R_APPEND)
			fd->outfile = open(ast->content, O_WRONLY
					| O_CREAT | O_APPEND, 0644);
		else
			fd->outfile = open(ast->content, O_WRONLY
					| O_CREAT | O_TRUNC, 0644);
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
		else if (ast->right && ast->right->type >= R_INPUT
			&& ast->right->done == 0)
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
