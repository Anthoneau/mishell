/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/18 17:10:46 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_cd(t_list **env, char *path)
{
	t_node	*current;

	current = (*env)->head;
	while (current && ft_strncmp(current->key, path, 5))
		current = current->next;
	if (!current)
		return (NULL);
	return (current->value);
}

int	go_to_home(t_list **env)
{
	char	*home;

	home = get_path_cd(env, "HOME");
	if (!home)
		return (print_e(1, "cd", 0, "HOME not set"), 1);
	if (chdir(home) == -1)
		return (print_error_cd(home), 1);
	if ((*env)->oldpd)
		ft_free((*env)->oldpd);
	if ((*env)->pwd)
	{
		(*env)->oldpd = ft_strdup((*env)->pwd);
		free((*env)->pwd);
	}
	if (home)
		(*env)->pwd = ft_strdup(home);
	update_pwd(env);
	return (0);
}

int	go_to_oldpwd(t_list **env, int output)
{
	char	*temp;

	if (!(*env)->oldpd || !get_path_cd(env, "OLDPWD"))
		return (print_e(1, "cd", 0, "OLDPWD not set"), 1);
	if (chdir((*env)->oldpd) == -1)
		return (print_error_cd((*env)->oldpd), 1);
	temp = (*env)->oldpd;
	(*env)->oldpd = (*env)->pwd;
	(*env)->pwd = temp;
	if ((*env)->pwd)
		ft_putendl_fd((*env)->pwd, output);
	update_pwd(env);
	return (0);
}

int	change_directory(char **arg, t_list **env)
{
	if (chdir(arg[1]) == -1)
		return (print_error_cd(arg[1]), 1);
	if ((*env)->oldpd)
		free((*env)->oldpd);
	(*env)->oldpd = ft_strdup((*env)->pwd);
	if ((*env)->pwd)
		free((*env)->pwd);
	(*env)->pwd = getcwd(NULL, 0);
	update_pwd(env);
	return (0);
}

int	cd(char **arg, t_list **env, int output)
{
	output = get_output(output);
	if (!arg[1] || (!arg[2] && ft_strsrch(arg[1], "~")))
		return (go_to_home(env));
	else if (arg[2])
		return (print_e(1, "cd", 0, "too many arguments"), 1);
	if (ft_strsrch(arg[1], "-"))
		return (go_to_oldpwd(env, output));
	return (change_directory(arg, env));
}
