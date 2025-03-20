/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_name.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 08:22:54 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/18 17:22:03 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_name(char *pwd, char *home)
{
	char	*name;
	char	*temp;
	int		i;

	i = 0;
	if (!pwd || !home)
		return (NULL);
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
	if (!env[i])
		return (NULL);
	while (env[i][j] != '=')
		j++;
	j++;
	return (env[i] + j);
}

char	*no_home(char *buf)
{
	char	*res;
	int		i;

	if (!buf)
		return (NULL);
	res = malloc(sizeof(char) * ft_strlen(buf) + 3);
	i = 0;
	while (buf[i])
	{
		res[i] = buf[i];
		i++;
	}
	res[i] = '$';
	i++;
	res[i] = ' ';
	i++;
	res[i] = '\0';
	return (free(buf), res);
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
	if (!res || !home)
		return (ft_free(res), ft_free(home), no_home(buf));
	free(buf);
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
	return (res);
}
