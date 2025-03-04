/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/04 17:58:25 by agoldber         ###   ########.fr       */
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
		return (print_error(1, "cd", 0, "HOME not set"), 1);
	if (chdir(home) == -1)
		return (print_error_cd(home), 1);
	free((*env)->oldpd->value);
	(*env)->oldpd->value = ft_strdup((*env)->pwd->value);
	free((*env)->pwd->value);
	(*env)->pwd->value = ft_strdup(home);
	return (0);
}

int	go_to_oldpwd(t_list **env)
{
	char	*oldpwd;
	char	*temp;

	oldpwd = get_path_cd(env, "OLDPWD");
	if (!oldpwd)
		return (print_error(1, "cd", 0, "OLDPWD not set"), 1);
	if (chdir((*env)->oldpd->value) == -1)
		return (print_error_cd((*env)->oldpd->value), 1);
	temp = (*env)->oldpd->value;
	(*env)->oldpd->value = (*env)->pwd->value;
	(*env)->pwd->value = temp;
	return (0);
}

int	change_directory(char **arg, t_list **env)
{
	if (chdir(arg[1]) == -1)
		return (print_error_cd(arg[1]), 1);
	free((*env)->oldpd->value);
	(*env)->oldpd->value = ft_strdup((*env)->pwd->value);
	free((*env)->pwd->value);
	(*env)->pwd->value = getcwd(NULL, 0);
	return (0);
}

int	cd(char **arg, t_list **env)
{
	if (!arg[1] || (!arg[2] && ft_strsrch(arg[1], "~")))
		return (go_to_home(env));
	else if (arg[2])
		return (print_error(1, "cd", 0, "too many arguments"), 1);
	if (ft_strsrch(arg[1], "-"))
		return (go_to_oldpwd(env));
	return (change_directory(arg, env));
}
