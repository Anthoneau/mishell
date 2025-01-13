/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:03:15 by agoldber          #+#    #+#             */
/*   Updated: 2024/12/04 16:37:37 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_token(t_token **token)
{
	t_token	*current;
	char	type_token[7][11] = {"WORD", "PIPE", "R_INPUT", "R_TRUNC", "R_HEREDOC", "R_APPEND"};

	current = *token;
	if (!current)
		printf("mon token ne peut pas etre affiche\n");
	while (current)
	{
		printf("type : %d ---", current->type);
		if (current->type == 0)
			printf(" %s\n", current->content);
		else
			printf(" %s\n", type_token[current->type]);
		if (current->next)
			current = current->next;
		else
			break ;
	}
}

void	display_node_ast(t_ast *ast, int i)
{
	(void) i;
	printf("content : %s\ntype : %d\ntop : %p\nleft : %p\nright : %p\n", ast->content, ast->type, ast->top, ast->left, ast->right);
}

void draw_ast(t_ast *node, int depth)
{
    if (!node)
        return;

    // Print indentation based on depth
    for (int i = 0; i < depth; i++)
        printf("    ");

    // Print the node content and type
    printf("Type: %d, Content: %s\n", node->type, node->content ? node->content : "(null)");

    // Recursively draw the left and right children
    if (node->left || node->right) {
        for (int i = 0; i < depth; i++)
            printf("    ");
        printf("Left:\n");
        draw_ast(node->left, depth + 1);

        for (int i = 0; i < depth; i++)
            printf("    ");
        printf("Right:\n");
        draw_ast(node->right, depth + 1);
    }
}

