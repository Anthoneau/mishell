/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:40:23 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/29 17:19:36 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_inpt(void)
{
	printf("minishell : error joining inputs\n");
}

char	*join_inpts(char *first_inpt, char *inpt)
{
	int		i;
	char	*res;
	char	*temp;

	i = ft_strlen(first_inpt);
	// printf("first_inpt : %s\nstrlen de first_inpt : %d\n", first_inpt, i);
	res = NULL;
	temp = NULL;
	if (inpt[i] == ' ')
	{
		// printf("on trouve un espace a inpt[%d] [%c]\n", i, inpt[i]);
		res = ft_strjoin(first_inpt, inpt);
		// printf("res (join) : %s\n", res);
	}
	else
	{
		// printf("on trouve pas d'espace\n");
		temp = ft_strjoin(first_inpt, " ");
		if (!temp)
			return (print_error_inpt(), NULL);
		// printf("temp (join 1) : %s\n", temp);
		res = ft_strjoin(temp, inpt);
		// printf("res (join 2) : %s\n", res);
		free(temp);
	}
	if (!res)
		return (print_error_inpt(), NULL);
	return (res);
}

int	end_pipe_handler(t_token **last_token, char **first_inpt, char **env)
{
	char	*inpt;
	char	*replace_inpt;
	t_token	*token;

	replace_inpt = NULL;
	inpt = readline("pipe > ");
	
	token = lexer(inpt);
	if (!token || !check_token(&token, &inpt, env))
	{
		if (token)
			free_token(&token);
		return (0);
	}

	// join_token_to_list(last_token, token);
	(*last_token)->next = token;
	token->prev = (*last_token);
	replace_inpt = join_inpts(*first_inpt, inpt);
	// printf("replace inpt : %s\n", replace_inpt);
	free(inpt);
	if (replace_inpt)
	{
		// printf("on free first_inpt\n");
		free(*first_inpt);
		(*first_inpt) = replace_inpt;
		// printf("on assigne replace_inpt a first_inpt\n");
		// printf("new inpt : %s\n", *first_inpt);
	}
	return (1);
}
