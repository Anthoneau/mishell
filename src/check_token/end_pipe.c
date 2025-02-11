/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:40:23 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 11:03:55 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_inpts(char *first_inpt, char *inpt)
{
	int		i;
	char	*res;
	char	*temp;

	i = ft_strlen(first_inpt);
	res = NULL;
	temp = NULL;
	if (inpt[i] == ' ')
		res = ft_strjoin(first_inpt, inpt);
	else
	{
		temp = ft_strjoin(first_inpt, " ");
		if (!temp)
			return (print_error(1, "pipe", "Cannot allocate memory"), NULL);
		res = ft_strjoin(temp, inpt);
		free(temp);
	}
	if (!res)
		return (print_error(1, "pipe", "Cannot allocate memory"), NULL);
	return (res);
}

int	end_pipe_handler(t_token **last_token, char **first_inpt, char **env)
{
	char	*inpt;
	char	*replace_inpt;
	t_token	*token;

	replace_inpt = NULL;
	inpt = readline("pipe > ");
	token = lexer(inpt, env);
	if (!token || !check_token(&token, &inpt, env))
	{
		if (token)
			free_token(&token);
		return (0);
	}
	(*last_token)->next = token;
	token->prev = (*last_token);
	replace_inpt = join_inpts(*first_inpt, inpt);
	free(inpt);
	if (replace_inpt)
	{
		free(*first_inpt);
		(*first_inpt) = replace_inpt;
	}
	return (1);
}
