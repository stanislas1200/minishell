/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:58:25 by sgodin            #+#    #+#             */
/*   Updated: 2023/09/14 16:10:18 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_send(int fd[2], char *text, t_data *data)
{
	write(fd[1], text, strlen(text));
	free(text);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (data->builtin)
	{
		clean(data);
		exit (0);
	}
}

void	heredoc_leave(t_data *data)
{
	if (data->builtin)
	{
		clean(data);
		if (data->r_break)
			exit (1);
		exit (0);
	}
	return ;
}

void	heredoc_child(t_data *data, char *path, int fd[2], t_ASTNode *s)
{
	char	*text;

	dup2(data->fdin, STDIN_FILENO);
	signal(SIGINT, SIG_DFL);
	text = heredoc_next(heredoc_loop(path, fd), data, s);
	if (s->right && s->right->type != CHAR_PIPE)
	{
		if (s->right->type != data->r && s->right->type != CHAR_INPUTR)
		{
			if (!check_heredoc(s->right))
			{
				write(fd[1], text, strlen(text));
				dup2(fd[0], STDIN_FILENO);
			}
		}
		close(fd[1]);
		close(fd[0]);
		free(text);
		ex_redirection(s->right, s->right->type, s->right->data, data);
		return (heredoc_leave(data));
	}
	heredoc_send(fd, text, data);
}
