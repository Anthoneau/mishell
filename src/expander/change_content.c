/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_content.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:31:01 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/16 14:43:36 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			// printf("on retourne NULL, on a rien trouve\n");
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
	// printf("on malloc de %d\n", end - start);
	env_value = malloc(end - start);
	if (!env_value)
		return (NULL);
	while (i <= end)
		env_value[j++] = content[i++];
	env_value[j] = '\0';
	i = find_env_i(env, start, end, env_value);
	free(env_value);
	if (i < 0)
		return (NULL);
	env_value = ft_strdup(env[i]);
	if (!env_value)
		return (NULL);
	// printf("on a trouve : %s\n", env_value);
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
