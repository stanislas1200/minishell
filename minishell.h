/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:41:15 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/17 04:01:14 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define MAXLINE 1024
# define MAXARGS 128
# define R "\x1b[1;31m"
# define G "\x1b[1;32m"
# define B "\x1b[1;36m"
# define Y "\x1b[1;33m"
# define M "\x1b[1;35m"
# define C "\x1b[0m"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>

# include <limits.h>
# include <errno.h>

// Structure
typedef struct t_token
{
	int				type;
	char			*data;
	struct t_token	*next;
}	t_token;

typedef struct t_lexer
{
	t_token	*tokens;
}	t_lexer;

typedef struct t_ASTNode // Abstract Syntax Tree Node
{
	int					type;
	char				*data;
	struct t_ASTNode	*left;
	struct t_ASTNode	*right;
}	t_ASTNode;

typedef struct t_data
{
	int			i;
	int			j;
	int			state;
	int			parse_end;
	int			last_exit;
	int			r_break;
	int			pipefd[2];
	char		**env;
	t_ASTNode	*ast_root;
}	t_data;

enum e_TOKEN_TYPE
{
	CHAR_NULL = 0,
	CHAR_CHAR = -1,
	CHAR_SPACE = ' ',
	CHAR_TAB = '\t',
	CHAR_NEWLINE = '\n',
	CHAR_PIPE = '|',
	CHAR_QUOTE = '\'',
	CHAR_DQUOTE = '\"',
	CHAR_INPUTR = '<',
	CHAR_OUTPUTR = '>',
	TOKEN = -1,
	ARG = -2
};

//utils
char		*free_join(char *s1, char *s2);

//builtins
int			cd(char ***envp, char **paths);
int			pwd(void);
int			echo(char **args);
int			env(char **envp);
int			export(char ***envp, char **args);
int			unset(char ***envp, char **args);
void		ft_exit(char **envp, char **args);

void		export2(char ***envp, char *var, int i, int append);
void		update_pwd(char ***envp);

void		ft_execve(char **env, char *cmd, char **args);
char		*find_command_path(char *all_paths, char *command);
char		*create_command_path(char *path, char *command);
void		delete_from_env(char ***envp, int del);

//env_utils
char		**dup_env(char **envp);
char		*ft_getenv(char **env, char *str);
int			ft_getindexenv(char **env, char *str);
void		shell_lvl(char ***envp);
void		update_env(char ***envp);

//main
void		signal_handler(int signum);

//builtin_tools
void		cperror(char *func, char *arg, char *error, int p_err);
void		free_matrix(char **str);
int			matrix_len(char **str);
long		ft_long_atoi(const char *nptr);

// Lexer
int			token_init(t_token *token, int size);
void		lexer_destroy(t_lexer *lexer);
void		strip_quotes(char *src, char *dest);
void		*lexer_malloc_error(t_lexer *lexer);
char		*expand_variables(char *input, t_data *data);
t_lexer		*lexer_build(char *str, t_data *data);
t_token		*token_new_next(t_token *token, t_data *data, int size);

// Parser
t_ASTNode	*parse(t_lexer *lexer, t_data *data);
t_ASTNode	*parse_top(t_token *token, t_data *data);
void		ast_destroy(t_ASTNode *node);
void		reorder_tree(t_ASTNode **root);

// Executer
int			execute_ast_node(t_ASTNode *node, t_data *data);

/* DEBUG */
void		lexer_print(t_lexer *lexer);
void		print_ast(t_ASTNode *root);

#endif
