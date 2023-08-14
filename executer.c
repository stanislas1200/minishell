/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 19:20:41 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/14 19:20:41 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	runSystemCommand(char *cmd, char **arr)
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
		if (execvp(cmd, arr) < 0) {
			perror("execvp error");
			exit(1);
		}
	}
	else { // Parent
		// BG
		// if (bg) {
		// 	printf("Child %d running in background\n", childPid);
		// 	return;
		// }
		// FG
		// else {
			signal(SIGINT, SIG_IGN);
			waitpid(childPid, NULL, 0);
			signal(SIGINT, signal_handler);
			return;
		// }
	}
}
int	execute_simple_command(t_ASTNode *node)
{
	// Count the number of arguments
	int num_args = 0;
	t_ASTNode *arg_node = node->right;
	while (arg_node != NULL) {
		num_args++;
		arg_node = arg_node->right;
	}

	// Create an array to hold the command and arguments
	char **arr = (char **)malloc((num_args + 2) * sizeof(char *));
	if (!arr) {
		perror("malloc");
		return -1;
	}

	// Fill in the array with the command and arguments
	arr[0] = node->data;
	int i = 1;
	arg_node = node->right;
	while (arg_node != NULL) {
		arr[i] = arg_node->data;
		i++;
		arg_node = arg_node->right;
	}
	arr[i] = NULL;

	// Execute the command
	// runSystemCommand(node->data, arr);
	execvp(node->data, arr);
	return -1;
}

int execute_pipe(t_ASTNode *node) {
	int fd[2];
	pid_t pid;
	
	// Create a pipe
	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}
	
	// Fork a child process
	pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		// Child process (right side of the pipe)
		close(fd[1]); // Close write end of the pipe
		dup2(fd[0], STDIN_FILENO); // Redirect stdin to pipe read end
		close(fd[0]); // Close pipe read end

		// Execute the right side of the pipe
		printf("right side of the pipe execute %s\n", node->right->data);
		execute_ast_node(node->right);

		exit(0);
	} else {
		// Parent process (left side of the pipe)
		printf("left side of the pipe\n execute %s\n", node->left->data);
		close(fd[0]); // Close read end of the pipe
		dup2(fd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
		close(fd[1]); // Close pipe write end

		// Execute the left side of the pipe
		execute_ast_node(node->left);

		// Wait for the child process to complete
		// wait(NULL);
	}
	
	return (0);
}

int	execute_ast_node(t_ASTNode *node)
{
	if (node == NULL) {
		return 0;
	}
	
	if (node->type == TOKEN)
	{
		return execute_simple_command(node);
	}
	else if (node->type == CHAR_PIPE) {
		return execute_pipe(node);
	}
	
	return (0);
}

