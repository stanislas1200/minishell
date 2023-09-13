/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:49:14 by sgodin            #+#    #+#             */
/*   Updated: 2023/09/04 16:49:14 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*heredoc_next(char *text, t_data *data, t_ASTNode *save)
{
	char	*new;

	if (!text)
		text = ft_strdup("");
	new = expand_variables(text, data);
	if (new)
	{
		free(text);
		text = new;
	}
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
	signal(SIGINT, signal_handler);
	return (text);
}

void	heredoc(char *path, t_data *data, t_ASTNode *s)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (data->builtin)
		pid = fork();
	if (pid == 0)
		heredoc_child(data, path, fd, s);
	if (data->builtin)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, signal_handler);
		if (WIFSIGNALED(status))
		{
			data->i = WTERMSIG(status);
			if (data->i == 2)
			{
				data->r_break = 1;
				data->last_exit = 1;
			}
		}
	}
}

void	output(char *path, t_data *data, t_ASTNode *s)
{
	int	fd;

	if (data->r == 3)
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		return (open_error(path, data, s));
	if (s->right && s->right->type != CHAR_PIPE && s->right->type != 4)
	{
		if (s->right->type != data->r)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		ex_redirection(s->right, s->right->type, s->right->data, data);
		return ;
	}
	if (s->right && s->right->type == 4)
	{
		data->pipefd[1] = fd;
		ex_redirection(s->right, s->right->type, s->right->data, data);
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	input(char *path, t_data *data, t_ASTNode *s)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (open_error(path, data, s));
	else if (s->right && s->right->type != CHAR_PIPE)
	{
		if (s->right->type != data->r && !check_heredoc(s->right))
			dup2(fd, STDIN_FILENO);
		close(fd);
		ex_redirection(s->right, s->right->type, s->right->data, data);
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	ex_redirection(t_ASTNode *save, int redir, char *path, t_data *data)
{
	data->r = redir;
	if (data->r_break && redir != 4)
	{
		if (save->right && check_heredoc(save->right))
			return (ex_redirection(save->right, save->right->type, \
			save->right->data, data));
		else
			return ;
	}
	if (data->r == 3 || data->r == CHAR_OUTPUTR)
		output(path, data, save);
	else if (data->r == CHAR_INPUTR)
		input(path, data, save);
	else if (data->r == 4)
		heredoc(path, data, save);
}
