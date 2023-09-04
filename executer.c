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

int	execute_builtin(t_ASTNode *node, char **arr, t_data *data)
{
	if (ft_strncmp(node->data, "pwd", ft_strlen("pwd") + 1) == 0)
		return (pwd());
	if (ft_strncmp(node->data, "cd", ft_strlen("cd") + 1) == 0)
		return (cd(&data->env, arr));
	if (ft_strncmp(node->data, "echo", ft_strlen("echo") + 1) == 0)
		return (echo(arr));
	if (ft_strncmp(node->data, "env", ft_strlen("env") + 1) == 0)
		return (env(data->env));
	if (ft_strncmp(node->data, "export", ft_strlen("export") + 1) == 0)
		return (export(&data->env, arr));
	if (ft_strncmp(node->data, "unset", ft_strlen("unset") + 1) == 0)
		return (unset(&data->env, arr));
	if (ft_strncmp(node->data, "exit", ft_strlen("exit") + 1) == 0)
		return (ft_exit(data->env, arr), 0);
	return (-1);
}

int	check_heredoc(t_ASTNode *node)
{
	t_ASTNode *temp = node;
	while (temp)
	{
		if (temp->type == CHAR_PIPE)
			if (check_heredoc(temp->left))
				return (1);
		if (temp->type == 4)
			return (1);
		temp = temp->right;
	}
	return (0);
}

char	**make_cmd_arr(t_ASTNode *node)
{
	int			num_args;
	t_ASTNode	*arg_node ;
	char		**arr = NULL;
	
	// Count the number of arguments
	num_args = 0;
	arg_node = node->right;
	while (arg_node)
	{
		num_args++;
		arg_node = arg_node->right;
	}

	// Create an array to hold the command and arguments
	arr = (char **)malloc((num_args + 2) * sizeof(char *));
	if (!arr)
	{
		perror("malloc");
		return (NULL);
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
	return (arr);
}

int	execute_cmd(t_ASTNode *node, t_data *data)
{
	// Check for input/output redirection
	int			redirection;
	char		*path;
	t_ASTNode	*save;
	char		**arr = NULL;

	redirection = 0;
	path = NULL;
	save = node;

	if (node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR || node->type == 3 || node->type == 4)
	{
		redirection = node->type;
		path = node->data;
		node = node->left;
	}

	if (data->ast_root == node)
	{
		execute_redirection(save, redirection, path, data);
		arr = make_cmd_arr(node);
		data->last_exit = execute_builtin(node, arr, data);
		free(arr);
		arr = NULL;
		if (data->last_exit != -1)
			return (data->last_exit);
	}

	// Fork a new process
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (free(arr), -1);
	}
	else if (pid == 0)
	{
		
		// Child process: execute the command
		execute_redirection(save, redirection, path, data); // Execute redirection if needed
		if (node)
		{
			arr = make_cmd_arr(node);
			data->last_exit = execute_builtin(node, arr, data);
			if (data->last_exit != -1)
			{
				ast_destroy(data->ast_root);
				free(arr);
				exit(data->last_exit);
			}
			if (node->type != TOKEN)
			{
				perror("command not found ?????"); // DEV
				data->last_exit = execute_ast_node(node, data); // Execute the command if it's not a cmd
				ast_destroy(data->ast_root);
				free(arr);
				exit(data->last_exit);
			}
			ft_execve(data, node->data, arr);
		}
		ast_destroy(data->ast_root);
		exit(data->last_exit);
	}
	else
	{
		// Parent process: wait for the child to complete
		int	status;
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, signal_handler);
		if (arr)
			free(arr);
		/* DEV TEST */ // move to execute_redirection
		while (save->right && (save->right->type == '>' || save->right->type == '<' || save->right->type == 3 || save->right->type == 4))
			save = save->right;
		if (save->right && save->right->type == '|')
			execute_ast_node(save->right, data); /* dev test*/
		return (WEXITSTATUS(status)); // Return the exit status of the child
	}
	return (0);
}


int	execute_pipe(t_ASTNode *node, t_data *data)
{
	/*FIX CAT*/
	
	// Create a pipe for communication between left and right sides
	int comm_pipe[2];
	if (pipe(comm_pipe) == -1)
	{
		perror("pipe");
		return -1;
	}
	/*			*/

	int status;
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return -1;
	}

	pid_t left_pid, right_pid;
	left_pid = fork();
	if (left_pid == -1)
	{
		perror("fork");
		return -1;
	}
	else if (left_pid == 0)
	{
		// Child process (left side of the pipe)
		close(pipefd[0]); // Close read end of the pipe
		if (check_heredoc(node->left))
		{
			data->pipefd[1] = pipefd[1];
		}
		else
		{
		dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
		close(pipefd[1]); // Close pipe write end
		}
		/*FIX CAT*/
		close(comm_pipe[1]); // Close write end of communication pipe
		if (check_heredoc(node->right) && !check_heredoc(node->left)) // wait for signal from the right side // ANYTIME OR ONLY IF HEREDOC ?  look good
        {
            // Wait for signal from the right side
            char signal_buf[1];
            read(comm_pipe[0], signal_buf, sizeof(signal_buf)); // Read signal from pipe
        }
		close(comm_pipe[0]); // Close read end of communication pipe
		/*		*/
		execute_ast_node(node->left, data);
		ast_destroy(data->ast_root);
		exit(data->last_exit);
	}
	else
	{
		if (check_heredoc(node->left))
			waitpid(left_pid, NULL, 0); // Wait for child process to complete
		right_pid = fork();
		if (right_pid == -1)
		{
			perror("fork");
			return -1;
		}
		else if (right_pid == 0)
		{
			// Child process (right side of the pipe)
			close(pipefd[1]); // Close write end of the pipe
			if (check_heredoc(node->right))
			{
				data->pipefd[0] = pipefd[0];
			}
			else
			{
				dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
				close(pipefd[0]); // Close pipe read end
			}
			execute_ast_node(node->right, data);

			/* FIX CAT */
            close(comm_pipe[0]); // Close read end of communication pipe
			if (check_heredoc(node->right) && !check_heredoc(node->left)) // Send signal to left side
				write(comm_pipe[1], "1", 1); // Send signal to left side
            close(comm_pipe[1]); // Close write end of communication pipe
			/*		*/
			ast_destroy(data->ast_root);
			exit(data->last_exit);
		}
		else
		{
			// Parent process: close both ends of the pipe
			close(pipefd[0]);
			close(pipefd[1]);
			close(comm_pipe[0]);
			close(comm_pipe[1]);

			// Wait for both child processes to complete
			signal(SIGINT, SIG_IGN);
			waitpid(left_pid, NULL, 0);
			waitpid(right_pid, &status, 0);
			signal(SIGINT, signal_handler);
		}
	}
	return (WEXITSTATUS(status));
}

int	execute_ast_node(t_ASTNode *node, t_data *data)
{
	if (!node)
		return (data->last_exit);
	if (node->type == CHAR_PIPE)
		data->last_exit = execute_pipe(node, data);
	else
		data->last_exit = execute_cmd(node, data);
	// update_env(&data->env);
	return (data->last_exit);
}
