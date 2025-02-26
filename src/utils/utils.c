/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:11:18 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/26 09:41:21 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token **token)
{
	t_token	*current;
	t_token	*to_free;

	if (!*token)
		return ;
	current = *token;
	while (current)
	{
		to_free = current;
		if (current->next)
			current = current->next;
		else
			break ;
		if (to_free)
		{
			if (to_free->content)
				free(to_free->content);
			free(to_free);
		}
	}
	if (to_free->content)
		free(to_free->content);
	free(to_free);
	token = NULL;
}

int	count_lst(t_token *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

void	free_error_node(t_ast *node)
{
	if (node && node->content)
		free(node->content);
	if (node && node->arg)
		free_array(node->arg);
	if (node)
		free(node);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast && ast->left)
		free_ast(ast->left);
	if (ast && ast->right)
		free_ast(ast->right);
	if (ast)
		free_error_node(ast);
}

void	ft_strcat(char *src, char *dst)
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
	dst[j] = '\0';
}
