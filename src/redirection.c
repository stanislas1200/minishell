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

void	heredoc(char *path, t_data *data, int redirection, t_ASTNode *s)
{
	int		fd[2];
	char	*text;

	dup2(data->fdin, STDIN_FILENO);
	signal(SIGINT, SIG_DFL);
	text = heredoc_next(heredoc_loop(path, fd), data, s);
	if (s->right && s->right->type != CHAR_PIPE)
	{
		if (s->right->type != redirection && s->right->type != CHAR_INPUTR)
		{
			if (!check_heredoc(s->right))
			{
				write(fd[1], text, strlen(text));
				dup2(fd[0], STDIN_FILENO);
			}
		}
		close(fd[1]);
		close(fd[0]);
		ex_redirection(s->right, s->right->type, s->right->data, data);
		return ;
	}
	write(fd[1], text, strlen(text));
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

void	output(char *path, t_data *data, int redirection, t_ASTNode *s)
{
	int	fd;

	if (redirection == 3)
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
		return (cperror("open", path, NULL, 1), exit(1));
	if (s->right && s->right->type != CHAR_PIPE && s->right->type != 4)
	{
		if (s->right->type != redirection)
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

void	input(char *path, t_data *data, int redirection, t_ASTNode *s)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		data->r_break = 1;
		if (s->right && check_heredoc(s->right))
			ex_redirection(s->right, s->right->type, s->right->data, data);
		ft_putstr_fd(M "-stanshell" C ": " Y, 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(C ": No such file or directory\n", 2);
		if (is_builtin(s->left))
			return ;
		exit(1);
	}
	else if (s->right && s->right->type != CHAR_PIPE)
	{
		if (s->right->type != redirection && !check_heredoc(s->right))
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
	if (data->r_break && redir != 4)
	{
		if (save->right && check_heredoc(save->right))
			return (ex_redirection(save->right, save->right->type, \
			save->right->data, data));
		else
			return ;
	}
	if (redir == 3 || redir == CHAR_OUTPUTR)
		output(path, data, redir, save);
	else if (redir == CHAR_INPUTR)
		input(path, data, redir, save);
	else if (redir == 4)
		heredoc(path, data, redir, save);
}
