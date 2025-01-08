/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:50:31 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/08 17:34:04 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

enum type
{
	WORD,
	PIPE,
	R_INPUT,
	R_TRUNC,
	R_HEREDOC,
	R_APPEND
};

typedef struct s_token
{
	char			*content;
	int				type;
	int				explored;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_ast
{
	char			*content;
	int				type;
	int				explored;
	int				error;
	struct s_ast	*top;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

void	ft_exit(char *str, int exit_code);
void	free_token(t_token **token);
void	free_ast(t_ast *ast);
t_token	*lexer(char *inpt);
void	new_token(char	*content, int type, t_token **lst, long *i);
void	create_word(char *inpt, long *i, t_token **token);
t_ast	*create_ast(t_token **tokens, t_token *current, int after_explored);
int		count_lst(t_token *lst);

//DISPLAY
void	display_token(t_token **token);
void	display_node_ast(t_ast *ast, int i);
void	draw_ast(t_ast *node, int depth);

#endif
//access cat X_OK