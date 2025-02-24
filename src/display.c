/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:03:15 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/24 18:41:40 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_token(t_token **token)
{
	t_token	*current;
	char	type_token[8][20] = {"WORD", "PIPE", "R_INPUT", "R_INPUT_TRUNC", "R_TRUNC", "R_TRUNC_NOCLOBBER", "R_HEREDOC", "R_APPEND"};

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
	int j = 0;

	if (ast->arg)
	{
		while (ast->arg[i])
		{
			printf("arg[%d]: %s\n", j, ast->arg[i]);
			i++;
		}
	}
	printf("type : %d\ntop : %p\nleft : %p\nright : %p\n", ast->type, ast->top, ast->left, ast->right);
}

void draw_ast(t_ast *node, int depth)
{
    if (!node)
        return;

    // Print indentation based on depth
    for (int i = 0; i < depth; i++)
	{
        printf("    ");
	}

    // Print the node content and type
	if (node->type != WORD)
	    printf("%sType%s: %d, Content: %s\n", BPURPLE, END, node->type, node->content ? node->content : "(null)");
	else
	{
		printf("%sType%s: %d\n", BPURPLE, END, node->type);
		int i = 0;
		while (node->arg[i])
		{
			printf("    arg[%d]: %s\n", i, node->arg[i]);
			i++;
		}
	}
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

void	display_cmds(t_cmdin cmd)
{
	int	i;

	i = 0;
	while (i < cmd.num_of_cmds)
	{
		printf("%scommand[%d] :%s\n", BPURPLE, i, END);
		if (cmd.cmd[i].content)
			printf("%s\tcontent -- %s%s\n", YELLOW, cmd.cmd[i].content, END);
		if (cmd.cmd[i].arg)
		{
			int j = 0;
			while (cmd.cmd[i].arg[j])
			{
				printf("%s\targ[%d] -- %s\n%s", YELLOW, j, cmd.cmd[i].arg[j], END);
				j++;
			}
		}
		printf("%s\tfd_in   -- %d%s\n", YELLOW, cmd.cmd[i].fd_in, END);
		printf("%s\tfd_out  -- %d%s\n", YELLOW, cmd.cmd[i].fd_out, END);
		i++;
	}
}
