/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:31:19 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/23 17:31:09 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_token_var(t_token *token, int type, t_token *current, t_token **lst)
{
	if (type == WORD || type == S_QUOTES || type == D_QUOTES)
	{
		token->type = WORD;
		if (type == S_QUOTES)
			token->expand = 0;
		else
			token->expand = 1;
	}
	else
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
	(*i)++;
	if (inpt[(*i) - 1] == '>')
	{
		if (inpt[*i] == '>')
		{
			new_token(NULL, R_APPEND, token, i);
			(*i)++;
		}
		else
		{
			if (inpt[*i] == '|')
			{
				new_token(NULL, R_TRUNC_NOCLOBBER, token, i);
				(*i)++;
			}
			else
			{
				new_token(NULL, R_TRUNC, token, i);
			}
		}
	}
	else if (inpt[(*i) - 1] == '<')
	{
		if (inpt[*i] == '<')
		{
			new_token(NULL, R_HEREDOC, token, i);
			(*i)++;
		}
		else
		{
			if (inpt[(*i)] == '>')
			{
				new_token(NULL, R_INPUT_TRUC, token, i);
				(*i)++;
			}
			else
			{
				new_token(NULL, R_INPUT, token, i);
			}
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
	//printf("check quotes\n");
	if (!inpt || !check_quote(inpt))
		return (ft_putstr_fd("minishell : quote error\n", 2), NULL);
	//printf("lexing...\n");
	while (inpt[i])
	{
		if (i < 0)
			return (free_token(&token), NULL);
		//printf("i = %ld\n", i);
		//printf("inpt[i] == [%c], il reste [%s]\n", inpt[i], inpt + i);
		while (inpt[i] == ' ')
			i++;
		//printf("skip des espaces potentiels\n");
		if (inpt[i] == '|')
		{
			// printf("on trouve un pipe\n");
			new_token(NULL, PIPE, &token, &i);
			i++;
		}
		else if (inpt[i] == '>' || inpt[i] == '<')
		{
			// printf("on trouve une redir\n");
			is_redir(inpt, &i, &token);
		}
		else if (inpt[i])
		{
			// printf("on trouve un mot\n");
			create_word(inpt, &i, &token);
		}
	}
	return (token);
}
