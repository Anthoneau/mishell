/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:51 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/04 16:59:33 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_cd(char *name)
{
	print_error(1, "cd", 0, NULL);
	if (errno == EACCES)
		print_error(0, name, 0, "Permission denied");
	else if (errno ==EIO)
		print_error(0, name, 0, "Input/output error");
	else if (errno == ELOOP)
		print_error(0, name, 0, "Too many levelsof symbolic links");
	else if (errno == ENAMETOOLONG)
		print_error(0, name, 0, "File name too long");
	else if (errno == ENOENT)
		print_error(0, name, 0, "No such file or directory");
	else if (errno == ENOMEM)
		print_error(0, name, 0, "Cannot allocate memory");
	else if (errno == ENOTDIR)
		print_error(0, name, 0, "Not a directory");
}

char	*get_path_cd(t_list **env, char *path)
{
	t_node	*current;

	current = (*env)->head;
	// printf("current est en tete\n");
	while (current && ft_strncmp(current->key, path, 5))
	{
		// printf("current->key : %s\n", current->key);
		current = current->next;
	}
	// printf("fin de boucle\n");
	if (!current)
		return (NULL);
	return (current->value);;
}

int	go_to_home(t_list **env)
{
	char	*home;

	home = get_path_cd(env, "HOME");
	if (!home)
		return (print_error(1, "cd", 0, "HOME not set"), 1);
	if (chdir(home) == -1)
		return (print_error_cd(home), 1);
	// printf("pwd : %s=%s\n", (*env)->pwd->key, (*env)->pwd->value);
	// printf("oldpwd : %s=%s\n\n", (*env)->oldpd->key, (*env)->oldpd->value);
	free((*env)->oldpd->value);
	(*env)->oldpd->value = ft_strdup((*env)->pwd->value);
	// (*env)->oldpd->value = NULL;
	free((*env)->pwd->value);
	(*env)->pwd->value = ft_strdup(home);
	// (*env)->pwd->value = NULL;
	// printf("pwd : %s=%s\n", (*env)->pwd->key, (*env)->pwd->value);
	// printf("oldpwd : %s=%s\n\n", (*env)->oldpd->key, (*env)->oldpd->value);
	return (0);
}

int	ft_strsrch(char	*str, char	*search)
{
	int	i;

	i = 0;
	if (!str || !search)
		return (0);
	while (str[i] && search[i])
	{
		if (str[i] != search[i])
			return (0);
		i++;
	}
	return (1);
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
	// printf("%s=%s\n", (*env)->pwd->key, (*env)->pwd->value);
	// printf("%s=%s\n\n", (*env)->oldpd->key, (*env)->oldpd->value);
	// free((*env)->oldpd->value);
	// (*env)->oldpd->value = ft_strdup((*env)->pwd->value);
	temp = (*env)->oldpd->value;
	(*env)->oldpd->value = (*env)->pwd->value;
	(*env)->pwd->value = temp;
	// free((*env)->pwd->value);
	// (*env)->pwd->value = ft_strdup(oldpwd);
	// printf("%s=%s\n", (*env)->pwd->key, (*env)->pwd->value);
	// printf("%s=%s\n\n", (*env)->oldpd->key, (*env)->oldpd->value);
	return (0);
}

int	change_directory(char **arg, t_list **env)
{
	// char	*buf;

	// buf = getcwd(NULL, 0);
	// printf("buf : %s\n", buf);
	// free(buf);
	if (chdir(arg[1]) == -1)
		return (print_error_cd(arg[1]), 1);
	// buf = getcwd(NULL, 0);
	// printf("buf : %s\n", buf);
	// free(buf);
	// printf("pwd : %s=%s\n", (*env)->pwd->key, (*env)->pwd->value);
	// printf("oldpwd : %s=%s\n\n", (*env)->oldpd->key, (*env)->oldpd->value);
	free((*env)->oldpd->value);
	(*env)->oldpd->value = ft_strdup((*env)->pwd->value);
	// (*env)->oldpd->value = NULL;
	free((*env)->pwd->value);
	// (*env)->pwd->value = ft_strdup(arg[1]);
	(*env)->pwd->value  = getcwd(NULL, 0);
	// (*env)->pwd->value = NULL;
	printf("pwd : %s=%s\n", (*env)->pwd->key, (*env)->pwd->value);
	printf("oldpwd : %s=%s\n\n", (*env)->oldpd->key, (*env)->oldpd->value);
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
