/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:33:34 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/25 16:26:39 by agoldber         ###   ########.fr       */
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
	new = ft_calloc(ft_strlen(content) - (end - start) + 1, sizeof(char));
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
	free(content);
	content = NULL;
	return (new);
}

void	transform_content(char **c, int *pos, char **env)
{
	int	start;
	int	end;

	start = *pos;
	end = start;
	while ((*c)[end])
	{
		if ((*c)[end] == ' ' || (*c)[end] == '|' || (*c)[end] == '='
			|| (*c)[end] == '<' || (*c)[end] == '>' || (*c)[end] == '/'
			|| (*c)[end] == '$' || (*c)[end] == '\'' || (*c)[end] == '.'
			|| (*c)[end] == '"' || (*c)[end] == '#' || (*c)[end] == ','
			|| (*c)[end] == ':' || (*c)[end] == '%'
			|| ((*c)[end] == '\\' && (*c)[end + 1] == '$'))
			break ;
		end++;
		if ((*c)[end - 1] && (((*c)[end - 1] >= '0'
			&& (*c)[end - 1] <= '9') || (*c)[end - 1] == '?'))
			break ;
	}
	if (is_in_env((*c), env, (size_t)(end - start), start))
		(*c) = change_content(*c, start, end, env);
	else if ((*c)[start] == '?')
		(*c) = change_g_exit_code(*c, start, end);
	else
		(*c) = supp_content(*c, start, end);
}

void	to_expand(char **content, char **env)
{
	int		i;
	int		d_q;

	i = 0;
	d_q = 0;
	while ((*content) && (*content)[i])
	{
		if ((*content)[i] == '"' && !d_q)
			d_q = 1;
		else if ((*content)[i] == '"' && d_q)
			d_q = 0;
		if ((*content)[i] == '$' && (!(*content)[i + 1]
				|| (*content)[i + 1] == ' ') && i++)
			continue ;
		else if ((*content)[i] == '$' && (i == 0 || ((*content)[i - 1]
			&& (*content)[i - 1] != '\\') || d_q))
		{
			i++;
			transform_content(content, &i, env);
		}
		else
			i++;
	}
}
