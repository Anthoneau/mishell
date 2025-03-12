/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_name.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:22:54 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/12 19:12:00 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_name(char *pwd, char *home)
{
	char	*name;
	char	*temp;
	int		i;

	i = 0;
	while (pwd[i] == home[i])
		i++;
	temp = ft_strjoin("~", pwd + i);
	if (!temp)
		return (NULL);
	i = 0;
	name = ft_strjoin(temp, "$ ");
	free(temp);
	if (!name)
		return (NULL);
	return (name);
}

char	*get_home(char **env)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "HOME", 5))
			break ;
		i++;
	}
	while (env[i][j] != '=')
		j++;
	j++;
	return (env[i] + j);
}

char	*minishell_name(t_data *data)
{
	char	*buf;
	char	*home;
	char	*res;
	int		i;

	buf = ft_strdup(data->env->pwd);
	if (!buf)
		return (NULL);
	home = get_home(data->env->env_c);
	res = create_name(buf, home);
	i = 0;
	while (res[i])
	{
		if (res[i] == '~')
		{
			i++;
			if (res[i] == '=')
				res[i] = '/';
		}
		i++;
	}
	free(buf);
	return (res);
}
