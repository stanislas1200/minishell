/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 16:39:14 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/13 14:07:17 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_header(void)
{
	printf("\n");
	printf(G "********************************************\n");
	printf("*                                          *\n");
	printf("*        " M "Welcome to My Custom Shell!" R "       *\n");
	printf("*                                          *\n");
	printf("********************************************\n" C);
	printf("\n");
}

void	print_exit_message(void)
{
	printf(R "Error: Shell has encountered an   \n");
	printf("                   		unexpected shutdown!\n");
}

int	parse(const char *cmdline, struct command_t *cmd)
{
	static char arry[MAXLINE]; // local copy of command line
	const char delims[10] = " \t\r\n"; // argument delimiters (white-space)
	char *line = arry; // pointer that traverses command line
	char *token; // pointer to the end of the current argument
	char *endline; // pointer to the end of the cmdline string
	int is_bg; // background job?

	if (cmdline == NULL) {
		printf(R "exit\n");
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

void	runSystemCommand(struct command_t *cmd, int bg)
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

void	eval(char *cmdLine)
{
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


void	signal_handler(int signum)
{
	if (signum == 2)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	*get_prompt()
{
	char	*str;
	char	cwd[1024];

	str = malloc(1024);
	getcwd(cwd, sizeof(cwd));
	strcat(str, "\x1b[1;32m");
	strcat(str, getenv("USER"));
	strcat(str, "\x1b[0m:\x1b[1;36m");
	strcat(str, cwd);
	strcat(str, "\x1b[0m$ \0");
	return (str);
}

int	main(void)
{
	char	*buff;
	char	*prompt;

	signal(SIGINT, signal_handler);
	print_header();
	prompt = get_prompt();
	while (1)
	{
		buff = readline(prompt);
		add_history(buff);
		eval(buff);
	}
	return (0);
}
