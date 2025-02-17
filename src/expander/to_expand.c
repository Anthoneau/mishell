/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:33:34 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/14 12:56:14 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*supp_content(char *content, int start, int end)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(ft_strlen(content) - (end - start));
	if (!new)
		return (NULL);
	while (content[i])
	{
		if (i < (start - 1) || i >= end)
		{
			new[j] = content[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	free(content);
	content = NULL;
	return (new);
}

void	transform_content(char **content, int *pos, char **env)
{
	int	start;
	int	end;

	start = *pos;
	end = start;
	while ((*content)[end])
	{
		if ((*content)[end] == ' ' || (*content)[end] == '|'
			|| (*content)[end] == '<' || (*content)[end] == '>'
			|| (*content)[end] == '$' || (*content)[end] == '\''
			|| (*content)[end] == '"'
			|| ((*content)[end] == '\\' && (*content)[end + 1] == '$'))
			break ;
		end++;
	}
	if (is_in_env((*content), env, (size_t)(end - start), start))
	{
		(*content) = change_content(*content, start, end, env);
	}
	else if ((*content)[start] == '?')
		(*content) = change_exit_code(*content, start, end);
	else
		(*content) = supp_content(*content, start, end);
}

void	to_expand(char **content, char **env)
{
	int		i;

	i = 0;
	while ((*content)[i])
	{
		if ((*content)[i] == '$' && (i == 0 || ((*content)[i - 1]
			&& (*content)[i - 1] != '\\' && (*content)[i - 1] != '\'')))
		{
			i++;
			transform_content(content, &i, env);
		}
		else
			i++;
	}
}
