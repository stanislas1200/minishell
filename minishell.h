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

//builtin
void	cd(char *path);

void	signal_handler(int signum);

#endif
