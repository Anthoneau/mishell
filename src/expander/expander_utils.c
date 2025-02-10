/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:29:11 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/10 08:57:44 by agoldber         ###   ########.fr       */
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
	// printf("----------contain backslah ?\ncontent :%s\n", content);
	while (content[i])
	{
		if (content[i] == '\\')
			count++;
		i++;
	}
	// printf("count : %d\n", count);
	return (count);
}

void	del_backslash(char *content)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	// printf("----------del_backslash\ncontent avant : %s\n", content);
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
	// printf("content apres : %s\n", content);
}

int	cmp_token(char *content, char *env, size_t len, size_t start)
{
	int	i;
	int	j;

	// printf("----------cmp_token\ncontent : %s\n*env : %s\n", content, env);
	if (len == 0)
	{
		// printf("len == 0, on retourne\n");
		return (0);
	}
	if (start != 0)
		i = start;
	else
		i = 0;
	// printf("i = %d\n", i);
	if (content[i] == '$')
	{
		// printf("[%c] donc on avance i\n", content[i]);
		i++;
	}
	j = 0;
	while (len && content[i] && env[j] && content[i] == env[j])
	{
		// printf("on compare : [%c] | [%c]\n", content[i], env[j]);
		i++;
		j++;
		len--;
	}
	if (len == 0 && env[j] == '=')
	{
		// printf("len == 0 et on trouve un [%c]\n", env[j]);
		return (1);
	}
	// printf("on trouve pas\n");
	return (0);
}

int	is_in_env(char *content, char **env, size_t len, int start)
{
	int	i;

	i = 0;
	// printf("----------is_in_env\non check si le contenu de content est dans env\n");
	while (env[i])
	{
		if (cmp_token(content, env[i], len, start))
		{
			// printf("oui\n");
			return (1);
		}
		i++;
	}
	// printf("non\n");
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
	// printf("----------expandable ?\ncontent : %s\n", content);
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
	// printf("count : %d\n", count);
	return (count);
}