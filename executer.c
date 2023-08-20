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
#include <fcntl.h>

int	execute_builtin(t_ASTNode *node, char **arr)
{
	if (ft_strncmp(node->data, "pwd", ft_strlen("pwd") + 1) == 0)
		return (pwd());
	if (ft_strncmp(node->data, "cd", ft_strlen("cd") + 1) == 0)
		return (cd(node->env, arr));
	if (ft_strncmp(node->data, "echo", ft_strlen("echo") + 1) == 0)
		return (echo(arr));
	if (ft_strncmp(node->data, "env", ft_strlen("env") + 1) == 0)
		return (env(*node->env));
	if (ft_strncmp(node->data, "export", ft_strlen("export") + 1) == 0)
		return (export(node->env, arr));
	if (ft_strncmp(node->data, "unset", ft_strlen("unset") + 1) == 0)
		return (unset(node->env, arr));
	if (ft_strncmp(node->data, "exit", ft_strlen("exit") + 1) == 0)
		return (ft_exit(*node->env, arr), 0);
	return (-1);
}

int	execute_cmd(t_ASTNode *node)
{
	// Check for input/output redirection
	int redirection = 0;
	char *path = NULL;
	t_ASTNode *save = node;
	printf("node->type = %d\n", node->type);
	if (node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR || node->type == 3 || node->type == 4) {
		redirection = node->type;
		path = node->data;
		node = node->left;
	}

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


	if (!execute_builtin(node, arr))
		return (0);
	// Fork a new process
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return (free(arr), -1);
	} else if (pid == 0) {
		// Child process: execute the command
		
	}
	else
	{
		// Parent process: wait for the child to complete
		int	status;
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, signal_handler);
		free(arr);
		return (WEXITSTATUS(status)); // Return the exit status of the child
	}
	return (0);
}


int	execute_pipe(t_ASTNode *node)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return -1;
	}

	pid_t left_pid, right_pid;
	left_pid = fork();
	if (left_pid == -1) {
		perror("fork");
		return -1;
	} else if (left_pid == 0) {
		// Child process (left side of the pipe)
		close(pipefd[0]); // Close read end of the pipe
		dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
		close(pipefd[1]); // Close pipe write end
		execute_ast_node(node->left);

		exit(0);
	}
	else
	{
		right_pid = fork();
		if (right_pid == -1)
		{
			perror("fork");
			return -1;
		} else if (right_pid == 0) {
			// Child process (right side of the pipe)
			close(pipefd[1]); // Close write end of the pipe
			dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
			close(pipefd[0]); // Close pipe read end

			execute_ast_node(node->right);

			exit(0);
		} else {
			// Parent process: close both ends of the pipe
			close(pipefd[0]);
			close(pipefd[1]);

			// Wait for both child processes to complete
			waitpid(left_pid, NULL, 0);
			waitpid(right_pid, NULL, 0);
		}
	}
	return (0);
}

void	execute_job(t_ASTNode *node)
{
	// printf("\nexecute_job : %s\n", node->data);
	if (node->type == CHAR_PIPE)
		execute_pipe(node);
	else if (node->type == TOKEN)
		execute_cmd(node);
	else if (node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR || node->type == 3 || node->type == 4)
		execute_cmd(node);
	update_env(node->env);
}

int	execute_ast_node(t_ASTNode *node)
{
	if (node == NULL) {
		return 0;
	}
	// ADD NODE TYPE
	// FG
		execute_job(node);
		// execute_ast_node(node->right);
	// BG 
	
	return (0);
}
