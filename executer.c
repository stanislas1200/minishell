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
	if (ft_strncmp(node->data, "pwd", ft_strlen("pwd")) == 0)
		return (pwd(), 0);
	if (ft_strncmp(node->data, "cd", ft_strlen("cd")) == 0)
		return (cd(node->env, arr), 0);
	if (ft_strncmp(node->data, "echo", ft_strlen("echo")) == 0)
		return (echo(arr), 0);
	if (ft_strncmp(node->data, "env", ft_strlen("env")) == 0)
		return (env(*node->env), 0);
	if (ft_strncmp(node->data, "export", ft_strlen("export")) == 0)
		return (export(node->env, arr), 0);
	if (ft_strncmp(node->data, "unset", ft_strlen("unset")) == 0)
		return (unset(node->env, arr), 0);
	return (1);
}

int	execute_cmd(t_ASTNode *node)
{
	// Check for input/output redirection
	int redirection = 0;
	char *path = NULL;
	t_ASTNode *save = node;
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
		return 0;
	// Fork a new process
	
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return (free(arr), -1);
	} else if (pid == 0) {
		// Child process: execute the command
		
		// Handle input redirection (<) // TODO multiple 
		if (redirection == CHAR_INPUTR) {
			int fd = open(path, O_RDONLY);
			if (fd == -1) {
				perror("open");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}

		// Handle input redirection (<<) heredoc /* DEV */ // TODO handle multiple heredoc cat<<EOF<<EOF output last EOF
		if (redirection == 4) {
			printf("heredoc\n");
			int fd[2];
			char *buffer = NULL;
			char *text = NULL;
			char *temp = NULL;
			pipe(fd);
			while (save && (save->type == 4))
			{
				while (1)
				{
					buffer = readline(G "> " C);
					if (strcmp(buffer, path) == 0)
						break;
					if (text == NULL)
					{
						text = ft_strdup(buffer);
					} else {
						char *temp = ft_strjoin(text, buffer);
						free(text);
						text = temp;
					}

					text = ft_strjoin(text, "\n");
					free(buffer);
				}
				if (save->right && (save->right->type == 4))
					text = NULL;
				save = save->right;
			}
			// Write the text to the pipe
			write(fd[1], text, strlen(text));
			// Close the write end of the pipe
			close(fd[1]);

			// Redirect STDIN_FILENO to the read end of the pipe
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
		}

		// Handle output redirection (>)
		if (redirection == CHAR_OUTPUTR) {
			int fd = 0;
			if (!save->right || (save->right->type != CHAR_OUTPUTR))
				fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			while (save->right && (save->right->type == CHAR_OUTPUTR))
			{
				if (fd > 0)
					close(fd);
				fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
				path = save->right->data;
				save = save->right;
				if (!save->right || (save->right->type != CHAR_OUTPUTR))
					fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			}
			if (fd == -1) {
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		// Handle output redirection (>>)
		if (redirection == 3) {
			int fd = 0;
			if (!save->right || (save->right->type != 3))
				fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
			while (save->right && (save->right->type == 3))
			{
				if (fd > 0)
					close(fd);
				fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
				path = save->right->data;
				save = save->right;
				if (!save->right || (save->right->type != 3))
					fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
			}
			if (fd == -1) {
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		execvp(node->data, arr);
		perror("execvp");
		free(arr);
		exit(1); // Exit the child process on execvp error
	}
	else
	{
		// Parent process: wait for the child to complete
		int	status;
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, signal_handler);
		free(arr);
		/* DEV TEST */
		while (save->right && (save->right->type == '>' || save->right->type == '<' || save->right->type == 3 || save->right->type == 4))
			save = save->right;
		if (save->right->type == '|');
			execute_ast_node(save->right); /* dev test*/
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
		// if (node->left->type != 4)
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
	if (node->type == CHAR_PIPE)
		execute_pipe(node);
	else if (node->type == TOKEN)
		execute_cmd(node);
	else if (node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR || node->type == 3 || node->type == 4)
		execute_cmd(node);
	
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
