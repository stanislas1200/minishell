/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:18:56 by sgodin            #+#    #+#             */
/*   Updated: 2023/09/04 19:48:54 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	left_child(t_ASTNode *node, int pipefd[2], int comm_pipe[2], t_data *data)
{
	char	signal_buf[1];

	data->in_pipe = 1;
	close(pipefd[0]);
	if (check_heredoc(node->left))
	{
		data->pipefd[1] = pipefd[1];
	}
	else
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	close(comm_pipe[1]);
	if (check_heredoc(node->right) && !check_heredoc(node->left) \
	&& node == data->ast_root && node->right->type == 4)
	{
		read(comm_pipe[0], signal_buf, sizeof(signal_buf));
	}
	close(comm_pipe[0]);
	execute_ast_node(node->left, data);
	clean(data);
	exit(data->last_exit);
}

int	right_child(t_ASTNode *node, int pipefd[2], int comm_pipe[2], t_data *data)
{
	data->in_pipe = 1;
	close(pipefd[1]);
	data->pipefd[0] = pipefd[0];
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	execute_ast_node(node->right, data);
	close(comm_pipe[0]);
	close(comm_pipe[1]);
	clean(data);
	exit(data->last_exit);
}

int	pipe_parent(int pipefd[2], int c_pipe[2], pid_t left_pid, pid_t right_pid)
{
	int		status;

	close(pipefd[0]);
	close(pipefd[1]);
	close(c_pipe[0]);
	close(c_pipe[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	signal(SIGINT, signal_handler);
	return (WEXITSTATUS(status));
}

int	execute_pipe(t_ASTNode *node, t_data *data)
{
	int		pipefd[2];
	int		comm_pipe[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(comm_pipe) == -1)
		return (perror("pipe"), 1);
	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	left_pid = fork();
	if (left_pid == -1)
		return (perror("fork"), 1);
	else if (left_pid == 0)
		return (left_child(node, pipefd, comm_pipe, data));
	else
	{
		if (check_heredoc(node->left))
			waitpid(left_pid, NULL, 0);
		right_pid = fork();
		if (right_pid == -1)
			return (perror("fork"), 1);
		else if (right_pid == 0)
			return (right_child(node, pipefd, comm_pipe, data));
	}
	return (pipe_parent(pipefd, comm_pipe, left_pid, right_pid));
}

void	process_parsing(t_data *data, t_lexer *lexer)
{
	data->ast_root = parse(lexer, data);
	lexer_destroy(lexer);
	if (data->ast_root)
	{
		execute_ast_node(data->ast_root, data);
		ast_destroy(data->ast_root);
	}
}
