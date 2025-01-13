/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:42:57 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/13 17:25:19 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_final_len(char *content, char *env)
{
	size_t	len_content;
	size_t	len_env;

	printf("\n-------------GET_FINAL_LEN-------------\n");
	if (!env || !*env)
	{
		printf("pas d'env donc len_env = 0\n");
		len_env = 0;
	}
	else
	{
		len_env = ft_strlen(env);
		printf("env donc len_env = %ld\n", len_env);
	}
	sleep(1);
	if (!content)
	{
		printf("pas de content donc len_content = 0\n");
		len_content = 0;
	}
	else
	{
		len_content = ft_strlen(content);
		printf("content donc len_content = %ld\n", len_content);
	}
	sleep(1);
	printf("res final : %ld\n", len_env + len_content);
	printf("----------------------------------------\n");
	sleep(1);
	return (len_env + len_content);
}

char	*get_extand_content(char *content)
{
	int		i;
	size_t	len;
	char	*res;

	i = 0;
	len = 0;
	printf("\n-------------EXTAND CONTENT-------------\n");
	printf("premiere partie\n");
	while (content[i])
	{
		printf("%c", content[i]);
		if (content[i] == ' ' || content[i] == '|' || content[i] == '<' || content[i] == '>')
			break ;
		i++;
	}
	printf("\n");
	sleep(1);
	printf("deuxieme partie\n");
	while (content[i])
	{
		printf("%c", content[i]);
		i++;
		len++;
	}
	printf("\n");
	sleep(1);
	res = malloc(len + 1);
	printf("malloc(len = [%ld])\ni = [%d]\n", len, i);
	if (!res)
		return (NULL);
	i -= len;
	len = 0;
	printf("content[i] : %c\n", content[i]);
	printf("content[i + 1] : %c\n", content[i + 1]);
	printf("on copie la deuxieme partie dans res\n");
	sleep(1);
	while (content[i])
		res[len++] = content[i++];
	res[len] = '\0';
	printf("res : %s\n", res);
	printf("-----------------------------------------\n\n");
	return (res);
}

void	change_content(char *content, char *env)
{
	char	*temp;
	size_t	len;

	printf("-------------CHANGE CONTENT-------------\n");
	len = 0;
	while (*env != '=')
		env++;
	env++;
	printf("on avance pour arriver apres le '=' : %s\n", env);
	sleep(1);
	temp = get_extand_content(content);
	free(content);
	if (!temp)
		return ;
	printf("on sort de get_extand_content...\ntemp = %s\n", temp);
	len = get_final_len(temp, env) + 1;
	printf("on sort de get_final_len...\n");
	content = calloc(len, sizeof(char));
	if (!content)
	{
		free(temp);
		return ;
	}
	if (!env && !temp)
	{
		free(content);
		content = ft_strdup(" ");
		return ;
	}
	printf("env : %s\n", env);
	if (env)
		ft_strlcpy(content, env, len);
	printf("temp : %s\n", temp);
	ft_strlcat(content, temp, len);
	printf("len : %ld\n", len);
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
		printf("len ; %ld\n", len);
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
