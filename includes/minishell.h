/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:50:31 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/20 17:12:35 by agoldber         ###   ########.fr       */
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
	R_INPUT, //<
	R_TRUNC, //>
	R_HEREDOC, //<<
	R_APPEND, // >>
	S_QUOTES,
	D_QUOTES
};

typedef struct s_token
{
	char			*content;
	int				expand;
	int				type;
	int				explored;
	int				error;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_ast
{
	char			*content;
	int				type;
	struct s_ast	*top;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

//UTILS
void	ft_exit(char *str, int exit_code);
void	free_token(t_token **token);
void	free_error_node(t_ast *node);
void	free_ast(t_ast *ast);
void	ft_strcat(char *src, char *dst);
void	ft_strcat_expander(char *src, char *dst);
int		count_lst(t_token *lst);

//LEXER
t_token	*lexer(char *inpt);
void	new_token(char	*content, int type, t_token **lst, long *i);
void	create_word(char *inpt, long *i, t_token **token);
char	*space_or_meta_char_delimitation(char *inpt, long *flag);

int		check_token(t_token **token);

//EXPANDER
int		expander(t_token **tokens, char **env);
int		expandable(char *content);
void	to_expand(char **content, char **env);
int		is_in_env(char *content, char **env, size_t len, int start);
int		cmp_token(char *content, char *env, size_t len, size_t start);
char	*change_content(char *content, int start, int end, char **env);
char	*supp_content(char *content, int start, int end);
int		contain_backslash(char *content);
void	del_backslash(char *content);

//PARSER
t_ast	*create_ast(t_token **tokens, t_token *current, int after_explored, int *error);
t_ast	*redir_node(t_token **tokens, t_token *current, int *error);
t_ast	*word_node(t_token *current, int *error);

//DISPLAY
void	display_token(t_token **token);
void	display_node_ast(t_ast *ast, int i);
void	draw_ast(t_ast *node, int depth);

#endif
//access cat X_OK