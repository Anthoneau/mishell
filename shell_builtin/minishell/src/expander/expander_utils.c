/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:29:11 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 10:30:47 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contain_backslash(char *content)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!content)
		return (0);
	while (content[i])
	{
		if (content[i] == '\\')
			count++;
		i++;
	}
	return (count);
}

void	del_backslash(char *content)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (content[i])
	{
		if (content[i] == '\\')
		{
			j = i;
			k = j + 1;
			while (content[j])
			{
				content[j] = content[k];
				j++;
				k++;
			}
		}
		i++;
	}
}

int	cmp_token(char *content, char *env, size_t len, size_t start)
{
	int	i;
	int	j;

	if (len == 0)
		return (0);
	if (start != 0)
		i = start;
	else
		i = 0;
	if (content[i] == '$')
		i++;
	j = 0;
	while (len && content[i] && env[j] && content[i] == env[j])
	{
		i++;
		j++;
		len--;
	}
	if (len == 0 && env[j] == '=')
		return (1);
	return (0);
}

int	is_in_env(char *content, char **env, size_t len, int start)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (cmp_token(content, env[i], len, start))
			return (1);
		i++;
	}
	return (0);
}

int	expandable(char *content)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (!content)
		return (0);
	while (content[i])
	{
		if (content[i] == '$')
		{
			if (i > 0 && content[i - 1] != '\\')
				count++;
			else if (i == 0)
				count++;
		}
		i++;
	}
	return (count);
}
