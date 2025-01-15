/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:42:57 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/15 18:05:25 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contain_backslash(char *content)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (content[i])
	{
		if (content[i] == '\\')
			count++;
		i++;
	}
	return (i);
}

void	del_backslash(char *content)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while (content[i])
	{
		if (content[i] == '\\')
		{
			j = i;
			k = j + 1;
			while (content[j])
			{
				content[j] = content[k];
				j++;
				k++;
			}
		}
		i++;
	}
}

// size_t	get_final_len(char *content, char *env)
// {
// 	size_t	len_content;
// 	size_t	len_env;

// 	// printf("\n-------------GET_FINAL_LEN-------------\n");
// 	if (!env || !*env)
// 	{
// 		// printf("pas d'env donc len_env = 0\n");
// 		len_env = 0;
// 	}
// 	else
// 	{
// 		len_env = ft_strlen(env);
// 		// printf("env donc len_env = %ld\n", len_env);
// 	}
// 	// sleep(1);
// 	if (!content)
// 	{
// 		// printf("pas de content donc len_content = 0\n");
// 		len_content = 0;
// 	}
// 	else
// 	{
// 		len_content = ft_strlen(content);
// 		// printf("content donc len_content = %ld\n", len_content);
// 	}
// 	// sleep(1);
// 	// printf("res final : %ld\n", len_env + len_content);
// 	// printf("----------------------------------------\n");
// 	// sleep(1);
// 	return (len_env + len_content);
// }

// char	*get_extand_content(char *content)
// {
// 	int		i;
// 	size_t	len;
// 	char	*res;

// 	i = 0;
// 	len = 0;
// 	// printf("\n-------------EXTAND CONTENT-------------\n");
// 	printf("premiere partie\n");
// 	if (content[i] == '$')
// 		i++;
// 	while (content[i])
// 	{
// 		printf("%%c[%c]\n", content[i]);
// 		if (content[i] == ' ' || content[i] == '|' || content[i] == '<' || content[i] == '>' || content[i] == '$')
// 			break ;
// 		i++;
// 	}
// 	// printf("\n");
// 	// sleep(1);
// 	printf("deuxieme partie\n");
// 	while (content[i])
// 	{
// 		printf("%%c[%c]\n", content[i]);
// 		i++;
// 		len++;
// 	}
// 	printf("fin\n");
// 	sleep(1);
// 	if (!len)
// 		return (NULL);
// 	res = malloc(len + 1);
// 	printf("malloc(len = [%ld])\ni = [%d]\n", len + 1, i);
// 	if (!res)
// 		return (NULL);
// 	i -= len;
// 	len = 0;
// 	// printf("content[i] : %c\n", content[i]);
// 	// printf("content[i + 1] : %c\n", content[i + 1]);
// 	// printf("on copie la deuxieme partie dans res\n");
// 	// sleep(1);
// 	while (content[i])
// 		res[len++] = content[i++];
// 	res[len] = '\0';
// 	// printf("res : %s\n", res);
// 	// printf("-----------------------------------------\n\n");
// 	return (res);
// }

// void	change_content(char **content, char *env, char **environment)
// {
// 	char	*temp;
// 	size_t	len;

// 	// printf("-------------CHANGE CONTENT-------------\n");
// 	len = 0;
// 	if (!env)
// 		return ;
// 	while (*env != '=')
// 		env++;
// 	env++;
// 	// printf("on avance pour arriver apres le '=' : %s\n", env);
// 	// sleep(1);
// 	temp = get_extand_content(*content);
// 	function_to_expand(&temp, environment);
// 	if (contain_backslash(temp))
// 		del_backslash(temp);
// 	free(*content);
// 	free(temp);
// 	// printf("on sort de get_extand_content...\ntemp = %s\n", temp);
// 	len = get_final_len(temp, env);
// 	// printf("on sort de get_final_len...\n");
// 	*content = calloc(len, sizeof(char));
// 	if (!*content)
// 		return ;
// 	// printf("env : %s\n", env);
// 	// printf("temp : %s\n", temp);
// 	// printf("len : %ld\n", len);
// 	ft_strcat_expander(env, *content);
// 	ft_strcat_expander(temp, *content);
// 	// printf("content : %s\n", *content);
// 	return ;
// }

int	cmp_token(char *content, char *env, size_t len, size_t start)
{
	int	i;
	int	j;

	if (len == 0)
		return (0);
	if (start != 0)
		i = start;
	else
		i = 1;
	if (content[i] == '$')
		i++;
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

// size_t	get_expander_len(char *content)
// {
// 	size_t	i;

// 	i = 0;
// 	if (content[i] == '$')
// 		i++;
// 	while (content[i])
// 	{
// 		if (content[i] == ' ' || content[i] == '|' || content[i] == '<' || content[i] == '>' || content[i] == '$')
// 			break ;
// 		i++;
// 	}
// 	return (i);
// }

// int	expandable(char *content)
// {
// 	int	i;
// 	int	count;

// 	count = 0;
// 	i = 0;
// 	while (content[i])
// 	{
// 		if (content[i] == '$')
// 		{
// 			if (content[i - 1] && content[i - 1] != '\\')
// 				count++;
// 			if (!content[i - 1])
// 				count++;
// 		}
// 		i++;
// 	}
// 	return (count);
// }

// void	function_to_expand(char **content, char **env)
// {
// 	size_t	len;
// 	int		i;

// 	len = 0;
// 	i = 0;
// 	printf("function to expand - content : %s\n", *content);
// 	// printf("le premier char est [$]\n");
// 	len = get_expander_len(*content) - 1;
// 	// printf("len ; %ld\n", len);
// 	printf("len = [%ld]\n", len);
// 	while (env[i])
// 	{
// 		// printf("on check %s\n", env[i]);
// 		if (cmp_token(*content, env[i], len) == 1)
// 		{
// 			// printf("les resultats sont similaires :\ncontent : %s\nenv[i] : %s\n\n", *content, env[i]);
// 			change_content(content, env[i], env);
// 			// printf("content dans la premiere fonction d'appel : %s\n", *content);
// 			return ;
// 		}
// 		i++;
// 	}
// 	// printf("on a rien trouve\n");
// 	free(*content);
// 	*content = ft_strdup("");
// }

char	*find_env_value(char **env, int start, int end, char *content)
{
	char	*env_value;
	int		i;
	int		j;

	i = start;
	j = 0;
	env_value = malloc(end - start + 1);
	if (!env_value)
		return (NULL);
	while (i <= end)
		env_value[j++] = content[i++];
	i = 0;
	while (env[i])
	{
		if (cmp_token(env_value, env, end - start, 0))
			break ;
		i++;
	}
	free(env_value);
	env_value = ft_strdup(env[i]);
	if (!env_value)
		return (NULL);
	return (env_value);
}

char	*change_content(char *content, int start, int end, char **env)
{
	char	*new;
	char	*env_value;
	int		i;
	int		j;
	size_t	len;

	env_value = find_env_value(env, start, end, content);
	if (!env_value)
		return (NULL);
	while (*env_value != '=')
		env_value++;
	env_value++;
	len = (ft_strlen(content) - (end - start)) + ft_strlen(env_value) + 1;
	new = malloc(len);
	if (!new)
	{
		free(env_value);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (content[i])
	{
		if (i >= start && i <= end)
		{
			ft_strcat_expander(new, env_value);
			i = end + 1;
			j += ft_strlen(env_value);
		}
		new[j] = content[i];
		i++;
		j++;
	}
	new[j] = '\0';
}

char	*supp_content(char *content, int start, int end)
{
	int	i;
	int	j;
	char *new;

	i = 0;
	j = 0;
	new = malloc(ft_strlen(content) - (end - start));
	if (!new)
		return (NULL);
	while (content[i])
	{
		if (i < start || i > end)
			new[j] = content[i];
		i++;
	}
	free(content);
	content = NULL;
	return (new);
}

int	is_in_env(char *content, char **env, size_t len, int start)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (cmp_token(content, env[i], len, start))
			return (1);
		i++;
	}
	return (0);
}

void	transform_content(char **content, int *pos, char **env)
{
	int	start;
	int	end;

	start = *pos + 1;
	end = *pos;
	while (*content[end])
	{
		if (*content[end] == ' ' || *content[end] == '|' || *content[end] == '<' || *content[end] == '>' || *content[end] == '$')
			break ;
		end++;
	}
	if (*content[end] != '\0')
		end--;
	if (is_in_env(*content, env, (size_t)end - start, start))
	{
		*content = change__content(*content, start, end, env);
	}
	else
	{
		*content = supp_content(*content, start, end);
	}
}

void	to_expand(char **content, char **env)
{
	size_t	len;
	int		i;
	int		j;
	int		count;

	len = 0;
	i = 0;
	j = 0;
	count = 0;
	while (*content[i])
	{
		if (*content[i] == '$' && (i == 0 || (*content[i - 1] && *content[i - 1] != '\\')))
		{
			transform_content(content, &i, env);
		}
		else
			i++;
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
			// printf("current->type = [%d]\nheredoc = 1\n", current->type);
			heredoc = 1;
		}
		if (current->type != R_HEREDOC && current->type != WORD)
		{
			// printf("current->type = [%d]\nheredoc = 0\n", current->type);
			heredoc = 0;
		}	
		if (!heredoc && current->type == WORD && current->expand == 1)
		{
			// printf("pas de heredoc, on trouve un mot et expand est ok !\n");
			// printf("content : %s\n\n", current->content);
			// function_to_expand(&current->content, env);
			// printf("content dans expander : %s\n", current->content);
			if (!current->content)
				return (0);
			else if (expandable(current->content))
				function_to_expand(&current->content, env);
			else if (contain_backslash(current->content))
				del_backslash(current->content);
			if (!current->content)
				return (0);
		}
		if (current->next)
		{
			// printf("on change de token\n");
			current = current->next;
		}
		else
			break ;
	}
	return (1);
}
