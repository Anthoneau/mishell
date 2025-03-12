/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 12:59:09 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/12 19:26:11 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_to_free(t_free to_free)
{
	if (*to_free.name)
		free(*to_free.name);
	if (*to_free.token)
		free_token(to_free.token);
	if (*to_free.name)
		free_ast(*to_free.ast);
}

int	is_delimitation(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '\''
		|| c == '"' || c == '&' || c == ';')
		return (1);
	return (0);
}

void	free_cmd(t_cmdin *cmd)
{
	int	i;

	if (!cmd || !cmd->cmd)
		return ;
	i = 0;
	while (cmd->cmd && i < cmd->num_of_cmds)
	{
		if (cmd->cmd[i].content)
			free(cmd->cmd[i].content);
		if (cmd->cmd[i].arg)
			free_array(cmd->cmd[i].arg);
		i++;
	}
	free(cmd->cmd);
	cmd->cmd = NULL;
}

void	print_open_error(char *content)
{
	if (errno == ENOENT)
		print_e(1, content, 0, "No such file or directory");
	else if (errno == EISDIR)
		print_e(1, content, 0, "Is a directory");
	else if (errno == EACCES)
		print_e(1, content, 0, "Permission denied");
	else if (errno == EMFILE || errno == ENFILE)
		print_e(1, NULL, 0, "Too many open files");
	else if (errno == EBUSY || errno == ETXTBSY)
		print_e(1, content, 0, "Text file busy");
	else
		print_e(1, NULL, 0, "Error opening file");
}
