/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:11:18 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/22 19:54:26 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char *str, int exit_code)
{
	if (str)
		ft_putstr_fd(str, 2);
	exit(exit_code);
}

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
	// if (j > 0)
	// {
	// 	dst[j] = ' ';
	// 	j++;
	// }
	//printf("strlen(dst) = %ld\ndst[j] = %c\n", j, dst[j]);
	//printf("src = %s\n", src);
	while (src[i])
	{
		//printf("on remplace [%c] par [%c]\n", dst[j], src[i]);
		dst[j] = src[i];
		//printf("dst[j] = %c\n", dst[j]);
		//printf("dst = %s\n", dst);
		j++;
		i++;
	}
	dst[j] = '\0';
	//printf("dst = %s\n", dst);
}

void	ft_strcat_expander(char *src, char *dst)
{
	size_t	i;
	size_t	j;

	// printf("strcat expander\n");
	if (!src)
		return ;
	// printf("src : %s\n", src);
	i = 0;
	if (!dst || !*dst)
		j = 0;
	else
		j = ft_strlen(dst);
	// printf("j : %ld\n", j);
	while (src[i])
	{
		// printf("src[%ld] == [%c]\n", i, src[i]);
		dst[j] = src[i];
		j++;
		i++;
	}
	// printf("dst : %s\n", dst);
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

void	print_error(int shell_name, char *content, char *message)
{
	if (shell_name)
		ft_putstr_fd("minishell: ", 2);
	if (content)
	{
		ft_putstr_fd(content, 2);
		ft_putstr_fd(": ", 2);
	}
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

void	free_to_free(t_free to_free)
{
	if (*to_free.name)
		free(*to_free.name);
	if (*to_free.token)
		free_token(to_free.token);
	if (*to_free.name)
		free_ast(*to_free.ast);
}

int	is_delimitation(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '\'' || c == '"' || c == '&' || c == ';')
		return (1);
	return (0);
}

void	free_cmd(t_cmd_info *cmd)
{
	int	i;

	if (!cmd || !cmd->cmd)
		return ;
	i = 0;
	while (cmd->cmd && i < cmd->num_of_cmds)
	{
		if (cmd->cmd[i].content)
			free(cmd->cmd[i].content);
		if (cmd->cmd[i].arg)
			free_array(cmd->cmd[i].arg);
		i++;
	}
	free(cmd->cmd);
	cmd->cmd = NULL;
}
