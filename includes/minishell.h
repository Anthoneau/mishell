/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:50:31 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/03 13:54:42 by agoldber         ###   ########.fr       */
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
# include <signal.h>
# include <stdbool.h>

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

enum e_type
{
	WORD,
	PIPE,
	R_INPUT,
	R_INPUT_TRUC,
	R_TRUNC,
	R_TRUNC_NOCLOBBER,
	R_HEREDOC,
	R_APPEND,
	S_QUOTES,
	D_QUOTES,
	PIPE2,
	AND,
	AND2,
	DOT,
	DOT2
};

typedef struct s_token
{
	char			*content;
	int				expand;
	int				type;
	int				explored;
	int				error;
	int				fd;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_ast
{
	char			*content;
	char			**arg;
	int				type;
	int				done;
	int				fd;
	struct s_ast	*top;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_cmd
{
	char	*content;
	char	**arg;
	int		fd_in;
	int		fd_out;
}	t_cmd;

typedef struct s_command_array_and_info
{
	t_cmd	*cmd;
	int		num_of_cmds;
}	t_cmdin;

typedef struct s_infile_outfile
{
	int	infile;
	int	outfile;
}	t_inout;

typedef struct s_free
{
	char	**name;
	t_token	**token;
	t_ast	**ast;
}	t_free;

typedef struct s_node
{
	char			*key;
	char			*value;
	struct s_node 	*next;
	struct s_node 	*prev;
}	t_node;

typedef struct s_list
{
	t_node			*head;
	t_node			*tail;
	t_node			*pwd;
	t_node			*oldpd;
	int				size;
}	t_list;

typedef struct s_data
{
	int		error;
	char	*name;
	char	*inpt;
	t_token	*token;
	t_ast	*ast;
	char	**env_list;
	t_list	*env;
}	t_data;

typedef struct s_exec
{
	char	*path;
	pid_t	*pid;
	int		i_pid;
	int		i;
	int		newpipefd[2];
	int		oldpipefd;
	int		builtin;
}	t_exec;

typedef struct s_array
{
	char	*arr1;
	char	**arr2;
}	t_arr;

//UTILS
char	*minishell_name(char **env);
void	free_token(t_token **token);
void	free_error_node(t_ast *node);
void	free_ast(t_ast *ast);
void	ft_strcat(char *src, char *dst);
void	ft_strcat_expander(char *src, char *dst);
int		count_lst(t_token *lst);
int		count_pipes(t_token *token);
void	print_error(int shell_name, char *content, int memory, char *message);
int		is_delimitation(char c);
char	**ft_arrdup(char **arr);
t_free	get_to_free(char **name, t_token **token, t_ast **ast);
void	free_to_free(t_free to_free);
void	free_cmd(t_cmdin *cmd);
t_token	*good_cur(t_token *current, int side, int type);
void	put_error_to_one(int *error);
void	print_open_error(char *content);
char	*ft_strldup(const char *s1, int len);
int		strllen(char *s, char c);
void	free_list(t_list *env);

//LEXER
t_token	*lexer(char *inpt, char **env);
void	pipe_token(char *inpt, long *i, t_token **token);
void	redir_token(char *inpt, long *i, t_token **token);
void	weird_token(char *inpt, long *i, t_token **token);
void	word_token(char *inpt, long *i, t_token **token, char **env);
void	new_token(char	*content, int type, t_token **lst, long *i);
void	create_word(char *inpt, long *i, t_token **token, char **env);
char	*space_or_meta_char_delimitation(char *inpt, long *flag);
char	*word_in_delimitation(char *inpt, char c, long *flag);

//CHECK TOKEN
int		check_token(t_token **token, char **inpt, char **env);
int		end_pipe_handler(t_token **lst_tok, char **frst_inpt, char **env);
int		do_heredoc(t_token *cur, char **env);
int		modify_inpt(char **inpt, char **env);
int		heredoc(char *delimiter, int expandble, char **env, int fd[2]);

//EXPANDER
int		expander(t_token **tokens, char **env);
int		expandable(char *content);
void	to_expand(char **content, char **env);
int		is_in_env(char *content, char **env, size_t len, int start);
int		cmp_token(char *content, char *env, size_t len, size_t start);
char	*change_content(char *content, int start, int end, char **env);
char	*change_g_exit_code(char *content, int start, int end);
char	*supp_content(char *content, int start, int end);
int		contain_backslash(char *content);
void	del_backslash(char *content);

//PARSER
t_ast	*create_ast(t_token **tkn, t_token *cur, int side, int *error);
t_ast	*redir_node(t_token **tokens, t_token *current, int *error);
t_ast	*word_node(t_token *current, int *error);

//BUILTINS
int		exit_builtin(char **arg, t_cmdin **cmd);
int		pwd(t_list *env);
int		echo(char **arg);
int		call_env(t_list *list);
char	**get_env(t_list *list);
int		export_order(t_list *list);
t_list	*make_list(char **env);
void	sort(char **arr, int size);
int		unset(t_list *list, char *s);

//EXEC
int		get_cmds_inputs(t_ast **current, t_inout *fd);
char	*right_path(char *content, t_list **env);
t_cmdin	get_cmd_array(t_ast *ast);
int		is_builtin(char *content);
int		do_builtins(char **arg, t_list **env);
int		exec_builtins(t_exec *exc, t_cmdin **cmd, t_list **env);
void	child_process(t_exec *exec, t_cmdin **cmd, t_list **env);
void	parent(t_exec *exec, t_cmdin *cmd);
void	exec(t_ast *ast, t_list **env);

//SIGNAL
void	signal_g_exit_code(int status, t_cmdin *cmd);
void	set_signal_action(int mode);

//DISPLAY
void	display_token(t_token **token);
void	display_node_ast(t_ast *ast, int i);
void	draw_ast(t_ast *node, int depth);
void	display_cmds(t_cmdin cmd);

#endif