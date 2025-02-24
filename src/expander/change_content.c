/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_content.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:31:01 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/24 17:49:55 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

int	find_env_i(char **env, int start, int end, char *env_value)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (cmp_token(env_value, env[i], end - start, 0))
			break ;
		i++;
		if (!env[i])
		{
			free(env_value);
			return (-1);
		}
	}
	return (i);
}

char	*find_env_value(char **env, int start, int end, char *content)
{
	char	*env_value;
	int		i;
	int		j;

	i = start;
	j = 0;
	env_value = ft_calloc((end - start) + 1, sizeof(char));
	if (!env_value)
		return (NULL);
	while (i <= end && content[i])
		env_value[j++] = content[i++];
	i = find_env_i(env, start, end, env_value);
	free(env_value);
	if (i < 0)
		return (NULL);
	env_value = ft_strdup(env[i]);
	if (!env_value)
		return (NULL);
	return (env_value);
}

char	*copy_in_new(char *content, char *env_value, int start, int end)
{
	char	*new;
	int		i;
	int		j;
	size_t	len;

	i = 0;
	j = 0;
	len = (ft_strlen(content) - (end - start + 1)) + ft_strlen(env_value) + 1;
	new = calloc(len, sizeof(char));
	if (!new)
		return (NULL);
	while (content[i])
	{
		if (i >= start - 1 && i < end)
		{
			ft_strcat_expander(env_value, new);
			i = end;
			j += ft_strlen(env_value);
		}
		else
			new[j++] = content[i++];
	}
	new[j] = '\0';
	return (new);
}

char	*change_content(char *content, int start, int end, char **env)
{
	char	*new;
	char	*env_value;
	char	*pointer;

	if (!content)
		return (NULL);
	pointer = find_env_value(env, start, end, content);
	if (!pointer)
		return (NULL);
	env_value = pointer;
	while (*env_value != '=')
		env_value++;
	env_value++;
	new = copy_in_new(content, env_value, start, end);
	free(pointer);
	free(content);
	return (new);
}

char	*change_exit_code(char *content, int start, int end)
{
	char		*new;
	char		*value;

	value = ft_itoa(exit_code);
	if (!value)
		return (NULL);
	new = copy_in_new(content, value, start, end);
	free(value);
	free(content);
	return (new);
}
