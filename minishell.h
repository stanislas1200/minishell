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
typedef struct command_t {
	char *argv[MAXARGS]; // argv for execve()
	int argc; // nb of args
	enum builtin_t { NONE, QUIT, JOBS, BG, FG, KILL, HELP } builtin; // type of builtin command
} command_t;

//export
void	cd(char ***envp, char **paths);
void	pwd(void);
void	echo(char **args);
void	env(char **envp);
void	export(char ***envp, char **args);

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

#endif
