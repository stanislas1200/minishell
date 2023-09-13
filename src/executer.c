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

#include "../minishell.h"

int	cmd_child(t_ASTNode *node, t_data *data)
{
	char	**arr;

	if (node)
	{
		arr = make_cmd_arr(node, node->right);
		if (!arr)
			return (data->last_exit = 1);
		data->last_exit = execute_builtin(node, arr, data);
		if (data->last_exit != -1)
		{
			clean(data);
			free(arr);
			exit(data->last_exit);
		}
		if (node->type != TOKEN)
		{
			data->last_exit = execute_ast_node(node, data);
			clean(data);
			free(arr);
			exit(data->last_exit);
		}
		ft_execve(data, node->data, arr);
	}
	clean(data);
	exit(data->last_exit);
}

int	cmd_check_b(t_ASTNode *node, int r, char *path, t_data *data)
{
	char		**arr;
	t_ASTNode	*save;

	save = node;
	if (r)
		node = node->left;
	if (!node || !is_builtin(node))
		return (0);
	data->builtin = 1;
	ex_redirection(save, r, path, data);
	data->builtin = 0;
	if (data->r_break)
	{
		dup2(data->fdin, STDIN_FILENO);
		dup2(data->fdout, STDOUT_FILENO);
		return (data->last_exit = 1, 1);
	}
	arr = make_cmd_arr(node, node->right);
	if (!arr)
		return (data->last_exit = 1);
	data->last_exit = execute_builtin(node, arr, data);
	free(arr);
	dup2(data->fdin, STDIN_FILENO);
	dup2(data->fdout, STDOUT_FILENO);
	return (arr = NULL, 1);
}

int	cmd_parent(t_ASTNode *save, t_data *data, pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, signal_handler);
	while (save->right && (save->right->type == '>' || save->right->type == '<' \
	|| save->right->type == 3 || save->right->type == 4))
		save = save->right;
	if (save->right && save->right->type == '|')
		execute_ast_node(save->right, data);
	if (WIFSIGNALED(status))
	{
		data->i = WTERMSIG(status);
		if (data->i == 3)
			return (printf(R "Quit" C ":" M "3\n" C), 131);
		if (data->i == 2 && save->type == 4)
			return (1);
		if (data->i == 2)
			return (130);
	}
	return (WEXITSTATUS(status));
}

int	execute_cmd(t_ASTNode *node, t_ASTNode *save, t_data *data)
{
	int			redirection;
	char		*path;
	pid_t		pid;

	redirection = 0;
	path = NULL;
	if (node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR \
	|| node->type == 3 || node->type == 4)
	{
		redirection = node->type;
		path = node->data;
		node = node->left;
	}
	if (data->ast_root->type != CHAR_PIPE)
		if (cmd_check_b(save, redirection, path, data))
			return (data->last_exit);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
	{
		ex_redirection(save, redirection, path, data);
		return (cmd_child(node, data));
	}
	return (cmd_parent(save, data, pid));
}

int	execute_ast_node(t_ASTNode *node, t_data *data)
{
	if (!node)
		return (data->last_exit);
	if (node->type == CHAR_PIPE)
		data->last_exit = execute_pipe(node, data);
	else
		data->last_exit = execute_cmd(node, node, data);
	return (data->last_exit);
}
