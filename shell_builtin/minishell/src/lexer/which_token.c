/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:47:45 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 10:18:23 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_token(char *inpt, long *i, t_token **token)
{
	(*i)++;
	if (inpt[(*i) - 1] == '>')
	{
		if (inpt[*i] == '>' && (*i)++)
			new_token(NULL, R_APPEND, token, i);
		else if (inpt[*i] == '|' && (*i)++)
			new_token(NULL, R_TRUNC_NOCLOBBER, token, i);
		else
			new_token(NULL, R_TRUNC, token, i);
	}
	else if (inpt[(*i) - 1] == '<')
	{
		if (inpt[*i] == '<' && (*i)++)
			new_token(NULL, R_HEREDOC, token, i);
		else if (inpt[(*i)] == '>' && (*i)++)
			new_token(NULL, R_INPUT_TRUC, token, i);
		else
			new_token(NULL, R_INPUT, token, i);
	}
}

void	weird_token(char *inpt, long *i, t_token **token)
{
	if (inpt[*i] == '&')
	{
		if (inpt[*i + 1] && inpt[*i + 1] == '&' && (*i)++)
			new_token(NULL, AND2, token, i);
		else
			new_token(NULL, AND, token, i);
		(*i)++;
	}
	else if (inpt[*i] == ';')
	{
		if (inpt[*i + 1] && inpt[*i + 1] == ';' && (*i)++)
			new_token(NULL, DOT2, token, i);
		else
			new_token(NULL, DOT, token, i);
		(*i)++;
	}
}

void	pipe_token(char *inpt, long *i, t_token **token)
{
	if (inpt[*i] == '|')
	{
		if (inpt[*i + 1] && inpt[*i + 1] == '|' && (*i)++)
			new_token(NULL, PIPE2, token, i);
		else
			new_token(NULL, PIPE, token, i);
		(*i)++;
	}
}
