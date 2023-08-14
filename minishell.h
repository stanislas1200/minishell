/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 15:41:15 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/14 10:32:53 by dspilleb         ###   ########.fr       */
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

enum e_TOKEN_TYPE
{
	CHAR_NULL = 0,
	CHAR_CHAR = -1,
	CHAR_SPACE = ' ',
	CHAR_PIPE = '|',
	CHAR_QUOTE = '\'',
	CHAR_DQUOTE = '\"',
	TOKEN = -1,
	ARG = -2
};

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
	int				type;
	char			*data;
	struct t_ASTNode	*left;
	struct t_ASTNode	*right;
}	t_ASTNode;

//builtin
void	cd(char *path);

void	signal_handler(int signum);

/* LEXING -> PARSING -> EXECUTING*/
t_lexer	*lexer_build(char *str);
t_ASTNode	*parse(t_lexer *lexer);
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
