/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:31:19 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/22 20:32:32 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_token_var(t_token *token, int type, t_token *curr, t_token **lst)
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
		curr = *lst;
		while (curr->next)
			curr = curr->next;
		curr->next = token;
		token->prev = curr;
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
		return (print_error(1, "malloc", "Cannot allocate memory"));
	}
	if (content)
	{
		token->content = ft_strdup(content);
		if (!token->content)
		{
			*i = -10;
			return (print_error(1, "malloc", "Cannot allocate memory"));
		}
	}
	else
		token->content = NULL;
	current = NULL;
	init_token_var(token, type, current, lst);
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

t_token	*lexer(char *inpt, char **env)
{
	t_token	*token;
	long	i;

	i = 0;
	token = NULL;
	if (!inpt || !check_quote(inpt))
		return (print_error(1, "quote", "error"), NULL);
	while (inpt[i])
	{
		if (i < 0)
			return (free_token(&token), NULL);
		while (inpt[i] == ' ')
			i++;
		if (inpt[i] == '|')
			pipe_token(inpt, &i, &token);
		else if (inpt[i] == '>' || inpt[i] == '<')
			redir_token(inpt, &i, &token);
		else if (inpt[i] == '&' || inpt[i] == ';')
			weird_token(inpt, &i, &token);
		else if (inpt[i])
			create_word(inpt, &i, &token, env);
	}
	return (token);
}
