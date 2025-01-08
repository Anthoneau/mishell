/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:31:19 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/08 16:30:28 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_token_var(t_token *token, int type, t_token *current, t_token **lst)
{
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	token->explored = 0;
	if (!*lst)
		*lst = token;
	else
	{
		current = *lst;
		while (current->next)
			current = current->next;
		current->next = token;
		token->prev = current;
	}
}

void	new_token(char	*content, int type, t_token **lst, long *i)
{
	t_token	*token;
	t_token	*current;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		*i = -10;
		return ;
	}
	if (content)
	{
		token->content = ft_strdup(content);
		if (!token->content)
		{
			*i = -10;
			return ;
		}
	}
	else
		token->content = NULL;
	current = NULL;
	init_token_var(token, type, current, lst);
}

void	is_redir(char *inpt, long *i, t_token **token)
{
	if (inpt[*i] == '>')
	{
		(*i)++;
		if (inpt[*i] != '>')
			new_token(NULL, R_TRUNC, token, i);
		else
		{
			new_token(NULL, R_APPEND, token, i);
			(*i)++;
		}
	}
	else if (inpt[*i] == '<')
	{
		(*i)++;
		if (inpt[*i] != '<')
			new_token(NULL, R_INPUT, token, i);
		else
		{
			new_token(NULL, R_HEREDOC, token, i);
			(*i)++;
		}
	}
}

int	check_quote(char *inpt)
{
	int	i;
	int	simple_q;
	int	double_q;

	i = 0;
	simple_q = 0;
	double_q = 0;
	while (inpt[i])
	{
		if (inpt[i] == '\'' && simple_q == 0 && double_q != 1)
			simple_q = 1;
		else if (inpt[i] == '\'' && simple_q == 1)
			simple_q = 0;
		if (inpt[i] == '"' && double_q == 0 && simple_q != 1)
			double_q = 1;
		else if (inpt[i] == '"' && double_q == 1)
			double_q = 0;
		i++;
	}
	if (simple_q == 1 || double_q == 1)
		return (0);
	return (1);
}

t_token	*lexer(char *inpt)
{
	t_token	*token;
	long	i;

	i = 0;
	token = NULL;
	if (!check_quote(inpt))
		return (ft_putstr_fd("Error with quotes\n", 2), NULL);
	while (inpt[i])
	{
		if (i < 0)
			return (free_token(&token), NULL);
		// printf("i = %ld\n", i);
		// printf("inpt[i] == [%c], il reste [%s]\n", inpt[i], inpt + i);
		while (inpt[i] == ' ')
			i++;
		if (inpt[i] == '|')
		{
			new_token(NULL, PIPE, &token, &i);
			i++;
		}
		else if (inpt[i] == '>' || inpt[i] == '<')
			is_redir(inpt, &i, &token);
		else if (inpt[i])
			create_word(inpt, &i, &token);
	}
	return (token);
}
