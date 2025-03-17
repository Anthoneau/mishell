/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:27:37 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/12 19:25:55 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_content(char *word, t_token **token, long *error)
{
	t_token	*current;
	char	*temp;

	current = *token;
	while (current)
	{
		if (current->next)
			current = current->next;
		else
			break ;
	}
	temp = ft_strdup(current->content);
	if (!temp)
	{
		*error = -10;
		return (print_e(1, "malloc", 1, ""));
	}
	free(current->content);
	current->content = ft_strjoin(temp, word);
	free(temp);
}

int	is_del_mod(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == ';')
		return (1);
	return (0);
}

int	is_heredoc(char *inpt, int i)
{
	i--;
	while (i > 0 && inpt[i] == ' ')
		i--;
	if (i > 0 && inpt[i] && inpt[i] == '<' && inpt[i - 1]
		&& inpt[i - 1] == '<')
		return (1);
	return (0);
}

int	get_word_type(char *inpt, long *i, char **word, int type)
{
	if (inpt[*i] && (inpt[*i] == '\'' || inpt[*i] == '"'))
	{
		if (inpt[*i] == '\'')
		{
			type = S_QUOTES;
			*word = word_in_delimitation(inpt + *i, '\'', i);
		}
		else if (inpt[*i] == '"')
		{
			type = D_QUOTES;
			*word = word_in_delimitation(inpt + *i, '"', i);
		}
		if (*i < 0)
			return (-1);
		*i += ft_strlen(*word) + 2;
	}
	else
	{
		*word = space_or_meta_char_delimitation(inpt + *i, i);
		if (*i < 0)
			return (-1);
		*i += ft_strlen(*word);
	}
	return (type);
}

void	create_word(char *inpt, long *i, t_token **token, char **env)
{
	char	*word;
	int		type;
	int		space;
	int		expandable;

	expandable = 1;
	type = WORD;
	if (inpt[*i] && is_heredoc(inpt, *i))
		expandable = 0;
	space = 0;
	if (*i > 0 && inpt[*i - 1] && inpt[*i - 1] != ' '
		&& !is_del_mod(inpt[*i -1]))
		space = 1;
	type = get_word_type(inpt, i, &word, type);
	if (type == S_QUOTES)
		expandable = 0;
	else if (type == -1)
		return ;
	if (expandable && !modify_inpt(&word, env))
		return (print_e(1, "malloc", 1, ""));
	if (space)
		join_content(word, token, i);
	else
		new_token(word, type, token, i);
	free(word);
}
