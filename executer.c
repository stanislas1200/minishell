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

int	heredoc(char *path, t_data *data)
{
	int fd[2];
	char *buffer = NULL;
	char *text = NULL;
	char *temp = NULL;
	pipe(fd);
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
	write(fd[1], text, strlen(text));
	close(fd[1]);
	return (fd[0]);
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

void	execute_redirection(t_ASTNode *save, int redirection, char *path, t_data *data) // TODO : handle all in one
{
	if (data->r_break && redirection != 4)
		{
			if (save->right && check_heredoc(save->right))
				return (execute_redirection(save->right, save->right->type, save->right->data, data));
			else
				return ;
		}
	// Handle output redirection (>) and (>>) append
	if (redirection == 3 || redirection == CHAR_OUTPUTR)
	{
		int fd = 0;
		if (redirection == 3)
			fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
		else
			fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd == -1)
			return (perror("open"), exit(1));
		if (save->right && save->right->type != CHAR_PIPE && save->right->type != 4)
		{
			if (save->right->type != redirection) // handle different redirections
				dup2(fd, STDOUT_FILENO);
			close(fd);
			execute_redirection(save->right, save->right->type, save->right->data, data);
			return ;
		}
		else
		{
			if (save->right && save->right->type == 4)
			{
				data->pipefd[1] = fd;
				execute_redirection(save->right, save->right->type, save->right->data, data);
			}
			else
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
	}
	// Handle input redirection (<)  
	if (redirection == CHAR_INPUTR) // TODO : handle with here doc ex: ls < file > R << EOF
	{
		int	fd;

		if (redirection == CHAR_INPUTR)
			fd = open(path, O_RDONLY);
		// else
		// 	fd = heredoc(path, data);
		if (fd == -1)
		{
			data->r_break = 1;
			if (save->right && check_heredoc(save->right))
				execute_redirection(save->right, save->right->type, save->right->data, data);
			ft_putstr_fd(M "-stanshell" C ": " Y, 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(C ": No such file or directory\n", 2);
			// return ;
			exit(1) ;
		}
		// if (data->pipefd) // 
		// {
		// 	dup2(data->pipefd, STDOUT_FILENO);
		// 	close(data->pipefd);
		// 	data->pipefd = 0;
		// }
		if (save->right && save->right->type != CHAR_PIPE)
		{
			if (save->right->type != redirection && !check_heredoc(save->right)) // handle different redirections
				dup2(fd, STDIN_FILENO);
			close(fd);
			execute_redirection(save->right, save->right->type, save->right->data, data);
			// if (data->r_break)
			// 	perror("open");
			return ;
		}
		else
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}

	if (redirection == 4)
	{
		int fd[2];
		char *buffer = NULL;
		char *text = NULL;
		char *temp = NULL;
		pipe(fd);
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
		if (!text)
			text = ft_strdup("");

		// EXPAND HERE DOC
		char *new;
		new = expand_variables(text, data);
		if (new)
		{	 // TODO : handle error
		free(text);
		text = new;}
		
		if (data->pipefd[1] && !check_heredoc(save->right))
		{
			dup2(data->pipefd[1], STDOUT_FILENO);
			close(data->pipefd[1]);
			data->pipefd[1] = 0;
		}
		if (data->pipefd[0] && !check_heredoc(save->right))
		{
			dup2(data->pipefd[0], STDIN_FILENO);
			close(data->pipefd[0]);
			data->pipefd[0] = 0;
		}
		if (save->right && save->right->type != CHAR_PIPE)
		{
			if (save->right->type != redirection && save->right->type != CHAR_INPUTR) // handle different redirections
			{
				if (!check_heredoc(save->right))
				{
					write(fd[1], text, strlen(text));
					dup2(fd[0], STDIN_FILENO);
				}
			}
			close(fd[1]);
			close(fd[0]);
			execute_redirection(save->right, save->right->type, save->right->data, data);
		}
		else
		{
			// Write the text to the pipe
			write(fd[1], text, strlen(text));
			// Close the write end of the pipe
			close(fd[1]);

			// Redirect STDIN_FILENO to the read end of the pipe
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
		}
	}

}

int	execute_cmd(t_ASTNode *node, t_data *data)
{
	// Check for input/output redirection
	int			redirection;
	char		*path;
	t_ASTNode	*save;
	int			num_args;
	t_ASTNode	*arg_node ;
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

	if (node)
	{
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
			return (-1);
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
		if (!redirection)
			data->last_exit = execute_builtin(node, arr, data);
		if (data->last_exit != -1 && !redirection)
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
			data->last_exit = execute_builtin(node, arr, data);
			if (data->last_exit != -1)
				exit(data->last_exit);
			if (node->type != TOKEN)
			{
				execute_ast_node(node, data); // Execute the command if it's not a cmd
				exit(1);
			}
			ft_execve(data->env, node->data, arr);
		}
		exit(0);
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
		execute_ast_node(node->left, data);
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
			exit(data->last_exit);
		}
		else
		{
			// Parent process: close both ends of the pipe
			close(pipefd[0]);
			close(pipefd[1]);

			// Wait for both child processes to complete
			signal(SIGINT, SIG_IGN);
			waitpid(left_pid, NULL, 0);
			waitpid(right_pid, &status, 0);	
			signal(SIGINT, signal_handler);
		}
	}
	return (WEXITSTATUS(status));
}

int	execute_job(t_ASTNode *node, t_data *data)
{
	if (node->type == CHAR_PIPE)
		data->last_exit = execute_pipe(node, data);
	else if (node->type == TOKEN)
		data->last_exit = execute_cmd(node, data);
	else if (node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR || node->type == 3 || node->type == 4)
		data->last_exit = execute_cmd(node, data);
	update_env(&data->env);
	return (1);
}

int	execute_ast_node(t_ASTNode *node, t_data *data)
{
	if (node == NULL) {
		return 0;
	}
	// ADD NODE TYPE
	// FG
	return (execute_job(node, data));
		// execute_ast_node(node->right);
	// BG 

}
