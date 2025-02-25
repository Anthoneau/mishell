/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 12:58:28 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/25 12:58:37 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strcat_expander(char *src, char *dst)
{
	size_t	i;
	size_t	j;

	if (!src)
		return ;
	i = 0;
	if (!dst || !*dst)
		j = 0;
	else
		j = ft_strlen(dst);
	while (src[i])
	{
		dst[j] = src[i];
		j++;
		i++;
	}
}

int	count_pipes(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->type == PIPE)
			i++;
		if (token->next)
			token = token->next;
		else
			break ;
	}
	return (i);
}

void	print_error(int shell_name, char *content, int memory, char *message)
{
	if (shell_name)
		ft_putstr_fd("minishell: ", 2);
	if (content)
	{
		ft_putstr_fd(content, 2);
		ft_putstr_fd(": ", 2);
	}
	if (memory)
		ft_putstr_fd("Cannot allocate memory", 2);
	if (message)
		ft_putendl_fd(message, 2);
}

char	**ft_arrdup(char **arr)
{
	char	**dest;
	int		i;

	i = 0;
	while (arr[i])
		i++;
	dest = malloc((i + 1) * sizeof(char *));
	if (!dest)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		dest[i] = ft_strdup(arr[i]);
		if (!dest[i])
			return (free_array(dest), NULL);
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

t_free	get_to_free(char **name, t_token **token, t_ast **ast)
{
	t_free	to_free;

	to_free.name = name;
	to_free.token = token;
	to_free.ast = ast;
	return (to_free);
}
