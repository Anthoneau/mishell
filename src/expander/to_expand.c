/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:33:34 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/16 14:43:28 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*supp_content(char *content, int start, int end)
{
	int	i;
	int	j;
	char *new;

	i = 0;
	j = 0;
	// printf("----------supp_content\non malloc %ld - (%d - %d)\ndonc %ld\n", ft_strlen(content), end, start, (ft_strlen(content) - (end - start)));
	new = malloc(ft_strlen(content) - (end - start));
	if (!new)
		return (NULL);
	while (content[i])
	{
		if (i < (start - 1) || i >= end)
		{
			// printf("on copie [%c]\n", content[i]);
			new[j] = content[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	// printf("new : %s\n", new);
	free(content);
	content = NULL;
	return (new);
}

void	transform_content(char **content, int *pos, char **env)
{
	int	start;
	int	end;

	// printf("----------transform content\n");
	start = *pos;
	end = start;
	while ((*content)[end])
	{
		if ((*content)[end] == ' ' || (*content)[end] == '|' || (*content)[end] == '<' || (*content)[end] == '>' || (*content)[end] == '$' || ((*content)[end] == '\\' && (*content)[end + 1] == '$'))
			break ;
		end++;
	}
	if (is_in_env((*content), env, (size_t)(end - start), start))
	{
		(*content) = change_content(*content, start, end, env);
	}
	else
	{
		(*content) = supp_content(*content, start, end);
	}
}

void	to_expand(char **content, char **env)
{
	int		i;

	i = 0;
	// printf("----------to_expand\n");
	while ((*content)[i])
	{
		if ((*content)[i] == '$' && (i == 0 || ((*content)[i - 1] && (*content)[i - 1] != '\\')))
		{
			i++;
			// printf("on trouve un [$] sans [\\]\n");
			transform_content(content, &i, env);
		}
		else
			i++;
	}
}
