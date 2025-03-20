/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:07:30 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/19 19:48:06 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_shell_value(t_node *ptr, char *c)
{
	free(ptr->value);
	ptr->value = ft_strdup(c);
	if (!ptr->value)
		return (1);
	return (0);
}

int	init_path(t_list *list)
{
	list->pwd = get_char(list, "PWD");
	if (!list->pwd)
		return (1);
	list->oldpd = get_char(list, "OLDPWD");
	return (0);
}

t_list	*env_i(t_list *list)
{
	char	*arr;
	char	*tmp;

	tmp = NULL;
	tmp = getcwd(tmp, 0);
	if (!tmp)
		return (free_list(list), NULL);
	arr = ft_strjoin("PWD=", tmp);
	free(tmp);
	if (!arr)
		return (free_list(list), NULL);
	if (get_list(list, arr) || get_list(list, "SHLVL=1")
		|| get_list(list, "_=/usr/bin/env"))
		return (free(arr), free_list(list), NULL);
	free(arr);
	list->pwd = get_char(list, "PWD");
	if (!list->pwd)
		return (free_list(list), NULL);
	return (list);
}

void	free_in_child(t_exec *exec, t_cmdin **cmd, t_list **env)
{
	ft_free(exec->path);
	free_cmd(*cmd);
	free_list(*env);
}

int	get_exit_code(char *cmd)
{
	if (!cmd[0])
		return (0);
	if ((cmd[0] == '/' || cmd[0] == '.') && access(cmd, F_OK) == 0)
		return (print_e(1, cmd, 0, "Is a directory"), 126);
	if (errno == EACCES)
		return (print_e(1, cmd, 0, "Permission denied"), 126);
	else if (errno == ENOENT && (cmd[0] == '/' || cmd[0] == '.'))
		return (print_e(1, cmd, 0, "No such file or directory"), 127);
	print_e(0, cmd, 0, "command not found");
	return (127);
}
