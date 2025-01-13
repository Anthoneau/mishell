/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:42:57 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/13 15:36:55 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_final_len(char *content, char *env)
{
	size_t	len_content;
	size_t	len_env;

	if (!env || !*env)
		len_env = 1;
	else
		len_env = ft_strlen(env);
	if (!content)
		len_content = 0;
	else
		len_content = ft_strlen(content);
	return (len_env + len_content);
}

char	*get_extand_content(char *content)
{
	int		i;
	size_t	len;
	char	*res;

	i = 0;
	len = 0;
	while (content[i])
	{
		if (content[i] == ' ' || content[i] == '|' || content[i] == '<' || content[i] == '>')
			break ;
		i++;
	}
	while (content[i])
	{
		i++;
		len++;
	}
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i -= len;
	len = 0;
	while (content[i])
		res[len++] = content[i++];
	return (res);
}

void	change_content(char *content, char *env)
{
	char	*temp;
	size_t	len;

	len = 0;
	while (*env != '=')
		env++;
	env++;
	temp = get_extand_content(content);
	free(content);
	if (!temp)
		return ;
	len = get_final_len(temp, env) + 1;
	content = malloc(len);
	if (!content)
	{
		free(temp);
		return ;
	}
	if (!env || !*env)
		ft_strlcpy(content, " ", len);
	else
		ft_strlcpy(content, env, len);
	ft_strlcat(content, temp, len);
	return ;
}

int	cmp_token(char *content, char *env, size_t len)
{
	int	i;
	int	j;

	if (len == 0)
		return (0);
	i = 1;
	j = 0;
	while (len && content[i] && env[j] && content[i] == env[j])
	{
		i++;
		j++;
		len--;
	}
	if (len == 0 && env[j] == '=')
		return (1);
	return (0);
}

size_t	get_expander_len(char *content)
{
	size_t	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == ' ' || content[i] == '|' || content[i] == '<' || content[i] == '>')
			break ;
		i++;
	}
	return (i);
}

void	function_to_expand(char *content, char **env)
{
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	if (*content == '$')
	{
		printf("le premier char est [$]\n");
		len = get_expander_len(content) - 1;
		printf("len = [%ld]\n", len);
		while (env[i])
		{
			printf("on check %s\n", env[i]);
			if (cmp_token(content, env[i], len) == 1)
			{
				printf("les resultats sont similaires :\ncontent : %s\nenv[i] : %s\n\n", content, env[i]);
				change_content(content, env[i]);
				return ;
			}
			i++;
		}
		printf("on a rien trouve\n");
		free(content);
		content = ft_strdup(" ");
		if (!content)
		{
			content = NULL;
			return ;
		}
	}
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
		{
			printf("current->type = [%d]\nheredoc = 1\n", current->type);
			heredoc = 1;
		}
		if (current->type != R_HEREDOC && current->type != WORD)
		{
			printf("current->type = [%d]\nheredoc = 0\n", current->type);
			heredoc = 0;
		}	
		if (!heredoc && current->type == WORD && current->expand == 1)
		{
			printf("pas de heredoc, on trouve un mot et expand est ok !\n");
			printf("content : %s\n\n", current->content);
			function_to_expand(current->content, env);
			if (!current->content)
				return (0);
		}
		if (current->next)
		{
			printf("on change de token\n");
			current = current->next;
		}
		else
			break ;
	}
	return (1);
}
