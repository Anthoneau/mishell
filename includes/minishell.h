/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:50:31 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/03 18:09:11 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>

# define RED "\e[0;31m"
# define GREEN "\e[0;32m"
# define YELLOW "\e[0;33m"
# define BLUE "\e[0;34m"
# define PURPLE "\e[0;35m"
# define CYAN "\e[0;36m"
# define WHITE "\e[0;37m"

# define BRED "\e[1;31m"
# define BGREEN "\e[1;32m"
# define BYELLOW "\e[1;33m"
# define BBLUE "\e[1;34m"
# define BPURPLE "\e[1;35m"
# define BCYAN "\e[1;36m"
# define BWHITE "\e[1;37m"

# define END "\e[0m"

enum type
{
	WORD,
	PIPE,
	R_INPUT, //<
	R_INPUT_TRUC, //<>
	R_TRUNC, //>
	R_TRUNC_NOCLOBBER, //>|
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
	int				done;
	struct s_ast	*top;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_cmd
{
	char	*content;
	int		fd_in;
	int		fd_out;
}	t_cmd;

typedef struct s_command_array_and_info
{
	t_cmd	*cmd;
	int		num_of_cmds;
}	t_cmd_info;

typedef struct s_infile_outfile
{
	int	infile;
	int	outfile;
}	t_inout;

//UTILS
void	ft_exit(char *str, int exit_code);
void	free_token(t_token **token);
void	free_error_node(t_ast *node);
void	free_ast(t_ast *ast);
void	ft_strcat(char *src, char *dst);
void	ft_strcat_expander(char *src, char *dst);
int		count_lst(t_token *lst);
int		count_pipes(t_token *token);

//LEXER
t_token	*lexer(char *inpt);
void	new_token(char	*content, int type, t_token **lst, long *i);
void	create_word(char *inpt, long *i, t_token **token);
char	*space_or_meta_char_delimitation(char *inpt, long *flag);

//CHECK TOKEN
int		check_token(t_token **token, char **inpt, char **env);
int		end_pipe_handler(t_token **last_token, char **first_inpt, char **env);

void	heredoc(char *delimiter, int expandble);

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

//EXEC
int	exec(t_ast *ast, char **env, int n);

//DISPLAY
void	display_token(t_token **token);
void	display_node_ast(t_ast *ast, int i);
void	draw_ast(t_ast *node, int depth);

#endif
//access cat X_OK