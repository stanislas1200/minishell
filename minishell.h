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

//export
void	cd(char ***envp, char **paths);
void	pwd(void);
void	echo(char **args);
void	env(char **envp);
void	export(char ***envp, char **args);


typedef struct command_t {
	char *argv[MAXARGS]; // argv for execve()
	int argc; // nb of args
	enum builtin_t { NONE, QUIT, JOBS, BG, FG, KILL, HELP } builtin; // type of builtin command
} command_t;


typedef struct t_token
{
	int				type;
	char			*data;
	struct t_token	*next;
} t_token;

typedef struct t_lexer
{
	t_token	*tokens;
} t_lexer;

typedef struct t_ASTNode // Abstract Syntax Tree Node
{
	int					type;
	char				*data;
	char				***env;
	struct t_ASTNode	*left;
	struct t_ASTNode	*right;
}	t_ASTNode;

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

//builtin_2
void	unset(char ***envp, char **args);
void	export2(char ***envp, char *var, int i, int append);
int		cd2(char ***envp, char **paths);

//builtin_tool1
char	**dup_env(char **envp);
void	delete_from_env(char ***envp, int del);
char	*ft_getenv(char **env, char *str);
int		ft_getindexenv(char **env, char *str);
void	add_to_env(char ***envp, char *str);

//builtin_tool2
int		valid_identifier(char *var);
void	check_identifier(char **envp, char *var, int *i, int *append);
char	*plus_remover(char *str);
void	update_pwd(char ***envp);
void	update_old_pwd(char ***envp, char *path);
void	ft_exit(char **envp, char **args);

//builtin_tool3
void	sort_env(char **envp);
int		is_option(char *str, char option);
int		check_exit(char *str);
int		is_numeric(char *str);

void	signal_handler(int signum);

//utils
void	cperror(char *error, char *arg, int p_err);
void	free_matrix(char **str);
int		matrix_len(char **str);
long	ft_long_atoi(const char *nptr);

/* LEXING -> PARSING -> EXECUTING*/
t_lexer	*lexer_build(char *str);
t_ASTNode	*parse(t_lexer *lexer, char ***env);
t_ASTNode	*parse_top(t_token *token, char ***env);
int	execute_ast_node(t_ASTNode *node);


enum e_NODE_TYPE // Node Type Enum (for AST) using bitwise operators to allow for multiple types per node
{
	NODE_PIPE = (1 << 0),
	NODE_ARGUMENT = (1 << 1),

	NODE_DATA = (1 << 2),
} NodeType;

/* DEBUG */
void lexer_print(t_lexer *lexer);
void print_ast(t_ASTNode *root);

#endif
