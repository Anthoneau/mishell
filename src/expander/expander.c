/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:42:57 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/16 15:06:23 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	modify_content(t_token **current, char **env)
{
	if (!(*current)->content)
		return (0);
	else if (expandable((*current)->content))
		to_expand(&(*current)->content, env);
	if (contain_backslash((*current)->content))
		del_backslash((*current)->content);
	if (!(*current)->content)
		return (0);
	return (1);
}

int	expander(t_token **tokens, char **env)
{
	t_token *current;
	int		heredoc;

	heredoc = 0;
	current = *tokens;
	while (current)
	{
		if (current->type == R_HEREDOC)
			heredoc = 1;
		else if (current->type != WORD)
			heredoc = 0;
		if (!heredoc && current->type == WORD && current->expand == 1
			&& ft_strlen(current->content) > 1
			&& !modify_content(&current, env))
				return (0);
		if (current->next)
			current = current->next;
		else
			break ;
	}
	return (1);
}
