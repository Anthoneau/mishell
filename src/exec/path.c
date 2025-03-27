/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:21:22 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/26 16:18:25 by agoldber         ###   ########.fr       */
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
	if (!full_env)
		return (NULL);
	while (full_env[i])
	{
		if (ft_strnstr(full_env[i], "PATH", 5))
		{
			env = ft_strdup(full_env[i]);
			if (!env)
				return (free_array(full_env), NULL);
			temp_env = env;
			while (*temp_env != '/')
				temp_env++;
			res = ft_split(temp_env, ':');
			free(env);
			return (free_array(full_env), res);
		}
		i++;
	}
	return (free_array(full_env), NULL);
}

char	*get_full_path(char *path, char *content)
{
	char	*temp;
	char	*res;

	if (*content == '/' || *content == '.')
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

t_arr	get_arr(char *content, t_list **env)
{
	t_arr	arr;

	arr.arr1 = NULL;
	arr.arr2 = NULL;
	if (!content)
		return (arr);
	arr.arr1 = get_cmd(content);
	if (!arr.arr1)
		return (arr);
	arr.arr2 = get_path(get_env(*env));
	if (!arr.arr2)
		return (free(arr.arr1), arr);
	return (arr);
}

char	*right_path(char *content, t_list **env)
{
	char	*full_path;
	t_arr	arr;
	int		i;

	full_path = NULL;
	if (!content || contains_space(content))
		return (NULL);
	arr = get_arr(content, env);
	if (!arr.arr1 || !arr.arr2)
		return (NULL);
	i = 0;
	while (!full_path || access(full_path, R_OK) == -1)
	{
		if (!arr.arr2[i])
			break ;
		ft_free(full_path);
		full_path = get_full_path(arr.arr2[i], arr.arr1);
		if (!full_path)
			return (free(arr.arr1), free_array(arr.arr2), NULL);
		i++;
	}
	free_array(arr.arr2);
	free(arr.arr1);
	return (full_path);
}
