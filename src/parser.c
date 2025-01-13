/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:18:52 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/13 09:59:34 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//t_token	*search_type(t_token **list, t_token *current, int type, int side)
//{
//	//left = 1; right = 0
//	int		i;

//	printf("\n----------SEARCH TYPE----------\n");
//	if (!current)
//	{
//		printf("current has no value\n");
//		current = *list;//check si la liste existe
//	}
//	i = 0;
//	if (side == 0 && current)
//	{
//		printf("side == 0 donc on check la droite\n");
//		// current = current->next;
//		while (current->explored != 1)
//		{
//			if (type < 2 && current->type == type && current->explored == 0)
//			{
//				printf("type cherché : %d\ncontent : %s\n", type, current->content);
//				// printf("i : %d\n", i);
//				return (current);
//			}
//			else if (type >= 2 && (current->type == 2 || current->type == 3
//					|| current->type == 4 || current->type == 5) && current->explored == 0)
//			{
//				printf("type cherché : %d\ncontent : %s\n", type, current->content);
//				// printf("i : %d\n", i);
//				return (current);
//			}
//			i++;
//			if (current->next)
//				current = current->next;
//			else
//				break ;
//		}
//		printf("rien trouve\n");
//	}
//	else if (side == 1 && current)
//	{
//		printf("side == 1 donc on check la gauche\n");
//		// current = current->prev;
//		while (current->explored != 1)
//		{
//			if (type == 0 && current->type == type && current->explored == 0)
//			{
//				printf("type cherché : %d\ncontent : %s\nOn est a gauche donc on check les args\n", type, current->content);
//				while (current->explored != 1)
//				{
//					if (current->prev && current->prev->type == 0 && current->explored == 0)
//					{
//						printf("on trouve un mot pas explore, on recule\n");
//						current = current->prev;
//					}
//					else
//						break ;
//				}
//				return (current);
//			}
//			if (type == 1 && current->type == type && current->explored == 0)
//			{
//				printf("type cherché : %d\ncontent : %s\n", type, current->content);
//				// printf("i : %d\n", i);
//				return (current);
//			}
//			else if (type >= 2 && (current->type == 2 || current->type == 3
//					|| current->type == 4 || current->type == 5) && current->explored == 0)
//			{
//				printf("type cherché : %d\ncontent : %s\n", type, current->content);
//				// printf("i : %d\n", i);
//				return (current);
//			}
//			i++;
//			if (current->prev)
//				current = current->prev;
//			else
//				break ;
//		}
//		printf("rien trouve\n");
//	}
//	printf("-------------------------------\n\n");
//	return (NULL);
//}

t_token	*find_first_word_to_the_left(t_token *current)
{
	while (current->explored != 1)
	{
		if (current->prev && current->prev->type == 0 && current->explored == 0)
			current = current->prev;
		else
			break ;
	}
	return (current);	
}

t_token	*search_type(t_token **list, t_token *current, int type, int side)
{
	if (!list)
		return (NULL);
	if (!current)
		current = *list;
	while (current->explored != 1)
	{
		if (type < 2 && current->type == type && current->explored == 0)
		{
			if (side == 0 || type == 1)
				return (current);
			else
				return(find_first_word_to_the_left(current));
		}
		else if (type >= 2 && current->type >= 2 && current->explored == 0)
			return (current);
		if (side == 0 && current->next)
			current = current->next;
		else if (side == 1 && current->prev)
			current = current->prev;
		else
			break ;
	}
	return (NULL);
}

//t_ast	*pipe_node(t_token **tokens, t_token *current, int *error)
//{
//	// int		j;
//	t_ast	*node;

//	printf("\n---------- PIPE NODE ----------\n");
//	sleep(1);
//	// j = 0;
//	node = malloc(sizeof(t_ast));
//	if (!node)
//	{
//		*error = 1;
//		return (NULL);
//	}
//	printf("malloc du noeud\ncurrent->type pointé : %d\n", current->type);
//	sleep(1);
//	current->explored = 1;
//	printf("PIPE exploré\n");
//	node->type = PIPE;
//	node->content = NULL;
//	node->top = NULL;
//	node->left = NULL;
//	node->right = NULL;
//	node->explored = 1;
//	printf("le noeud est initialisé\n");
//	display_node_ast(node, 1);
//	printf("creation du noeud de gauche\n");
//	sleep(1);
//	node->left = create_ast(tokens, current->prev, 1, error);
//	if (*error == 1)
//	{
//		printf("error == 1\n");
//		if (node->content)
//			free(node->content);
//		if (node)
//			free(node);
//		return (NULL);
//	}
//	if (node->left)
//	{
//		node->left->top = node;
//		printf("on assigne node->left->top a node\n");
//	}
//	printf("le noeud a gauche a été créé\n");
//	display_node_ast(node, 1);
//	printf("creation du noeud de droite\n");
//	node->right = create_ast(tokens, current->next, 0, error);
//	if (*error == 1)
//	{
//		printf("error == 1\n");
//		if (node->content)
//			free(node->content);
//		if (node)
//			free(node);
//		return (NULL);
//	}
//	if (node->right)
//	{
//		node->right->top = node;
//		printf("on assigne node->right->top a node\n");
//	}
//	printf("le noeud a droite a été créé\n");
//	display_node_ast(node, 1);
//	printf("-------------------------------\n\n");
//	return (node);
//}

t_ast	*pipe_node(t_token **tokens, t_token *current, int *error)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	current->explored = 1;
	node->type = PIPE;
	node->content = NULL;
	node->top = NULL;
	node->left = NULL;
	node->right = NULL;
	node->left = create_ast(tokens, current->prev, 1, error);
	if (*error == 1)
		return (free_error_node(node), NULL);
	if (node->left)
		node->left->top = node;
	node->right = create_ast(tokens, current->next, 0, error);
	if (*error == 1)
		return (free_error_node(node), NULL);
	if (node->right)
		node->right->top = node;
	return (node);
}

//t_ast	*redir_node(t_token **tokens, t_token *current, int *error)
//{
//	t_ast	*node;
//	// int		j;

//	printf("\n---------- REDIRNODE ----------\n");
//	// current = *tokens;
//	node = NULL;
//	current->explored = 1;
//	printf("REDIR exploré\n");
//	node = malloc(sizeof(t_ast));
//	if (!node)
//	{
//		*error = 1;
//		return (NULL);
//	}
//	node->type = current->type;
//	node->content = NULL;
//	node->top = NULL;
//	node->left = NULL;
//	node->right = NULL;
//	node->explored = 1;
//	printf("noeud initialisé\n");
//	printf("type trouvé : %d\n", current->type);
//	if (current->type == R_TRUNC || current->type == R_APPEND || current->type == R_HEREDOC)
//	{
//		printf("> || >> || <<\n");
//		if (current && current->next)
//		{
//			node->content = ft_strdup(current->next->content);
//			if (!node->content)
//				*error = 1;
//			printf("lisaison du fichier \"%s\"\n", current->next->content);
//			current->next->explored = 1;
//			printf("fichier lié exploré\n");
//		}
//		else
//		{
//			node->content = ft_strdup("stdout");
//			if (!node->content)
//				*error = 1;
//			printf("fichier \"stdout\"\n");
//		}
//			printf("creation du noeud de gauche\n");
//		node->left = create_ast(tokens, current->prev, 1, error);
//		if (*error == 1)
//		{
//			printf("error == 1\n");
//			if (node->content)
//				free(node->content);
//			if (node)
//				free(node);
//			return (NULL);
//		}
//		if (node->left)
//		{
//			node->left->top = node;
//			printf("on assigne node->left->top a node\n");
//		}
//		printf("le noeud a gauche a été créé\n");
//		display_node_ast(node, 1);
//		printf("creation du noeud de droite\n");
//		node->right = create_ast(tokens, current->next->next, 0, error);
//		if (*error == 1)
//		{
//			printf("error == 1\n");
//			if (node->content)
//				free(node->content);
//			if (node)
//				free(node);
//			return (NULL);
//		}
//		if (node->right)
//		{
//			node->right->top = node;
//			printf("on assigne node->right->top a node\n");
//		}
//		printf("le noeud a droite a été créé\n");
//		display_node_ast(node, 1);
//	}
//	else if (current->type == R_INPUT)
//	{
//		printf("<\n");
//		if (current && current->prev)
//		{
//			node->content = ft_strdup(current->prev->content);
//			if (!node->content)
//				*error = 1;
//			printf("liaison du fichier \"%s\"\n", current->prev->content);
//			current->prev->explored = 1;
//			printf("fichier lié exploré\n");
//		}
//		else
//		{
//			node->content = NULL;
//			printf("fichier NULL\n");
//		}
//			printf("creation du noeud de gauche\n");
//		node->left = create_ast(tokens, current->prev->prev, 1, error);
//		if (*error == 1)
//		{
//			printf("error == 1\n");
//			if (node->content)
//				free(node->content);
//			if (node)
//				free(node);
//			return (NULL);
//		}
//		if (node->left)
//		{
//			node->left->top = node;
//			printf("on assigne node->left->top a node\n");
//		}
//		printf("le noeud a gauche a été créé\n");
//		display_node_ast(node, 1);
//		printf("creation du noeud de droite\n");
//		node->right = create_ast(tokens, current->next, 0, error);
//		if (*error == 1)
//		{
//			printf("error == 1\n");
//			if (node->content)
//				free(node->content);
//			if (node)
//				free(node);
//			return (NULL);
//		}
//		if (node->right)
//		{
//			node->right->top = node;
//			printf("on assigne node->right->top a node\n");
//		}
//		printf("le noeud a droite a été créé\n");
//		display_node_ast(node, 1);
//	}
//	if (*error == 1)
//	{
//		if (node->content)
//			free(node->content);
//		free(node);
//		return (NULL);
//	}
//	printf("-------------------------------\n\n");
//	return (node);
//}

void	create_content(t_token *current, t_ast *node, int *error)
{
	if (current->type == R_INPUT && current->prev)
	{
		node->content = ft_strdup(current->prev->content);
		if (!node->content)
			*error = 1;
		current->prev->explored = 1;
	}
	else if (current->next)
	{
		node->content = ft_strdup(current->next->content);
		if (!node->content)
			*error = 1;
		current->next->explored = 1;
	}
	else
		node->content = NULL;
}

int	create_content_and_sides(t_token **tokens, t_token *current, t_ast *node, int *error)
{
	create_content(current, node, error);
	if (current->type == R_INPUT)
	{
		node->left = create_ast(tokens, current->prev->prev, 1, error);
		node->right = create_ast(tokens, current->next, 0, error);
	}
	else
	{
		node->left = create_ast(tokens, current->prev, 1, error);
		node->right = create_ast(tokens, current->next->next, 0, error);
	}
	if (*error == 1)
		return (0);
	if (node && node->left)
		node->left->top = node;
	if (node && node->right)
	node->right->top = node;
	return (1);
}

t_ast	*redir_node(t_token **tokens, t_token *current, int *error)
{
	t_ast	*node;

	node = NULL;
	current->explored = 1;
	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	node->type = current->type;
	node->content = NULL;
	node->top = NULL;
	node->left = NULL;
	node->right = NULL;
	if (!current || !create_content_and_sides(tokens, current, node, error))
	{
		*error = 1;
		return (free_error_node(node), NULL);
	}
	return (node);
}

//char	*get_word(t_token *current)
//{
//	char	*res;
//	char	*temp;

//	printf("--all_cmd--\n");
//	temp = NULL;
//	res = ft_strjoin(current->content, " ");
//	printf("res : %s\n", res);
//	if (!res)
//		return (NULL);//check
//	printf("avant boucle\n");
//	printf("current content; type; explored : %s; %d; %d\n", current->content, current->type, current->explored);
//	while (current->explored == 0)
//	{
//		current->explored = 1;
//		printf("mot exploré\n");
//		if (current->next)
//		{
//			printf("on avance dans la liste\n");
//			current = current->next;
//			printf("lets goo\n");
//		}
//		else
//			break ;
//		if (current->explored)
//			break ;
//		printf("current->type et content : %d %s\n", current->type, current->content);
//		printf("current->explored : %d\n", current->explored);
//		temp = ft_strjoin(res, current->content);
//		printf("temp : %s\n", temp);
//		if (!temp)
//			return (free(res), NULL);//check
//		free(res);
//		res = ft_strjoin(temp, " ");
//		printf("res : %s\n", res);
//		if (!res)
//			return (free(temp), NULL);//check
//		free(temp);
//		temp = NULL;
//	}
//	printf("apres boucle\n");
//	if (temp)
//		free(temp);
//	return (res);
//}

size_t	get_len(t_token *current)
{
	t_token *words;
	size_t	len;

	len = 0;
	words = current;
	while (words->type == 0 && words->content)
	{
		len += ft_strlen(words->content) + 1;
		if (words->next && words->next->type == 0 && words->next->content)
			words = words->next;
		else
			break ;
	}
	return (len);
}

char	*get_word(t_token *current)
{
	char	*res;
	t_token	*words;
	size_t	len;

	if (!current || !current->content)
		return (NULL);
	words = current;
	len = get_len(current);
	res = ft_calloc(len, sizeof(char));
	if (!res)
		return (NULL);
	while (words->type == 0 && words->content)
	{
		ft_strcat(words->content, res);
		if (words->next && words->next->type == 0 && words->next->content)
			words = words->next;
		else
			break ;
	}
	return (res);
}

t_ast	*word_node(/*t_token **token, */t_token *current, int *error)
{
	t_ast	*node;

	//printf("\n---------- WORD NODE ----------\n");
	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	node->type = WORD;
	node->content = get_word(current);
	//printf("content : %s\n", node->content);
	//sleep(1);
	if (!node->content)
	{
		*error = 1;
		return (free(node), NULL);
	}
	node->top = NULL;
	node->right = NULL;
	node->left = NULL;
	current->explored = 1;
	//printf("noeud initialisé\n");
	//printf("\n-------------------------------\n");
	return (node);
}

t_ast	*create_ast(t_token **tokens, t_token *current, int after_explored, int *error)
{
	t_token	*check;
	t_ast	*node;

	//printf("error == %d\n", *error);
	//sleep(1);
	node = NULL;
	if ((check = search_type(tokens, current, PIPE, after_explored)) && *error == 0)
	{
		//printf("on trouve un pipe\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = pipe_node(tokens, check, error);
	}
	else if ((check = search_type(tokens, current, 2, after_explored)) && *error == 0)
	{
		//printf("on trouve une redir\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = redir_node(tokens, check, error);
	}
	else if ((check = search_type(tokens, current, WORD, after_explored)) && *error == 0)
	{
		//printf("on trouve un mot\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = word_node(check, error);
	}
	//printf("on check error\n");
	if (*error == 1)
	{
		//printf("error == 1 donc on free ce qu'il faut");
		return (free_error_node(node), NULL);
	}
	//printf("error == 0\n");
	return (node);
}
