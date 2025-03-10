/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:14:09 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/10 14:49:42 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_current_pipe(t_ast **current)
{
	while ((*current)->type == PIPE)
	{
		if ((*current)->left && ((*current)->left->done == 0
				|| (*current)->left->type == PIPE))
			(*current) = (*current)->left;
		else if ((*current)->right && ((*current)->right->done == 0
				|| (*current)->right->type == PIPE))
			(*current) = (*current)->right;
	}
}

int	get_arg_and_fd(t_ast **current, t_cmdin **cmd, t_inout fd, int i)
{
	(*current)->done = 1;
	(*cmd)->cmd[i].arg = ft_arrdup((*current)->arg);
	if (!(*cmd)->cmd[i].arg)
		return (0);
	(*cmd)->cmd[i].fd_in = fd.infile;
	(*cmd)->cmd[i].fd_out = fd.outfile;
	return (1);
}

int	create_cmds_array(t_cmdin *cmd, t_ast *ast)
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

t_cmdin	get_cmd_array(t_ast *ast)
{
	t_cmdin	cmd;

	cmd.num_of_cmds = get_cmd_number(ast);
	if (!cmd.num_of_cmds)
		cmd.num_of_cmds = 1;
	cmd.cmd = ft_calloc(cmd.num_of_cmds + 1, sizeof(t_cmd));
	if (!cmd.cmd)
		return (cmd);
	if (!create_cmds_array(&cmd, ast))
		return (free_cmd(&cmd), cmd);
	return (cmd);
}
