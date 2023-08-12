/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 16:39:14 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/12 18:22:01 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAXLINE 1024

#define MAXARGS 128

int shellId;

typedef struct command_t {
	char *argv[MAXARGS]; // argv for execve()
	int argc; // nb of args
	enum builtin_t { NONE, QUIT, JOBS, BG, FG, KILL, HELP } builtin; // type of builtin command
} command_t;


#define R     "\x1b[1;31m"
#define G   "\x1b[1;32m"
#define B   "\x1b[1;36m"
#define Y   "\x1b[1;33m"
#define M   "\x1b[1;35m"
#define C   "\x1b[0m"
void print_header() {
    printf("\n");
    printf(G "********************************************\n");
    printf("*                                          *\n");
    printf("*        " M "Welcome to My Custom Shell!" R "       *\n");
    printf("*                                          *\n");
    printf("********************************************\n" C);
    printf("\n");
}

void print_exit_message() {
    printf(R "Error: Shell has encountered an   \n");
    printf("                   		unexpected shutdown!\n");
}

int parse(const char *cmdline, struct command_t *cmd)
{
	static char arry[MAXLINE]; // local copy of command line
	const char delims[10] = " \t\r\n"; // argument delimiters (white-space)
	char *line = arry; // pointer that traverses command line
	char *token; // pointer to the end of the current argument
	char *endline; // pointer to the end of the cmdline string
	int is_bg; // background job?

	if (cmdline == NULL) {
		perror("Error reading the command line");
		exit(1);
	}
	strncpy(line, cmdline, MAXLINE);
	endline = line + strlen(line);

	// initialize arguments
	cmd->argc = 0;

	while (line < endline)
	{
		// skip delimiters
		line += strspn(line, delims);
		if (line >= endline) break;

		// Find token delimiter
		token = line + strcspn(line, delims);

		// terminate the token
		*token = '\0';

		// save the token
		cmd->argv[cmd->argc++] = line;

		// Check full
		if (cmd->argc >= MAXARGS-1) break;
		line = token + 1;
	}

	//end NULL
	cmd->argv[cmd->argc] = NULL;

	if (cmd->argc == 0)  // ignore blank line
		return 0;

	cmd->builtin = NONE;

	// should the job run in the background?
	if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)
		cmd->argv[--cmd->argc] = NULL;

	return is_bg;
	
}

void runSystemCommand(struct command_t *cmd, int bg)
{
	pid_t childPid;
	//FORK
	if ((childPid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if (childPid == 0) { // Child run cmd
		// EXECVP
		if (execvp(cmd->argv[0], cmd->argv) < 0) {
			perror("execvp error");
			exit(1);
		}
	}
	else { // Parent
		// BG
		if (bg) {
			printf("Child %d running in background\n", childPid);
			return;
		}
		// FG
		else {
			waitpid(childPid, NULL, 0);
			return;
		}
	}
}

void eval(char *cmdLine) {
	int bg;
	struct command_t cmd;

	bg = parse(cmdLine, &cmd); // parse command line into command structure
	if (bg == -1) // parsing error
		return;
	if (cmd.argv[0] == NULL) // ignore empty lines
		return;
	if (strcmp(cmd.argv[0], "exit") == 0) { // exit command
		print_exit_message();
		exit(0);
	}
	if (cmd.builtin == NONE)
		runSystemCommand(&cmd, bg);
	// else
	// 	runBuiltinCommand(&cmd);
}


void signal_handler(int signum) {
	if (signum == 2 && atoi(getenv("shellId")) == shellId)
	{
		printf("exit\n");
		exit(1);
	}
    printf("Received signal: %d\n", signum);
	
}

int main() {
	shellId = atoi(getenv("shellId"));
    signal(SIGINT, signal_handler);
	char cmdLine[MAXLINE]; // buffer for fgets
	print_header();
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	char str[1024] = "\x1b[1;32m";
	strcat(str, getenv("USER"));
	// strcat(str, "@");
	// strcat(str, getenv("NAME"));
	strcat(str, "\x1b[0m:\x1b[1;36m");
	strcat(str, cwd);
	strcat(str, "\x1b[0m$ \0");
	while (1) {
		// dir
		// printf("\x1b[1;32m%s@%s\x1b[0m:\x1b[1;34m%s\x1b[0m$ ", getenv("USER"), getenv("NAME"), cwd);
		// printf("\x1b[1;32m%s@%s\x1b[0m:\x1b[1;36m%s\x1b[0m$ ", getenv("USER"), getenv("NAME"), cwd);
		// if (fgets(cmdLine, MAXLINE, stdin) == NULL) // read command line
		// 	; // EOF from stdin, terminate the program
		// if (feof(stdin)) // EOF from stdin, terminate the program
		// 	exit(0);
		char * buff;
		buff = readline(str);
		add_history(buff);
		// process the command line
		eval(buff);
	}
	return 0;
}
