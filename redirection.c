#include "minishell.h"

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
	return (text);
}

char	*heredoc_loop(char *path, int fd[2])
{
	char	*buffer;
	char	*text;
	char	*temp;

	text = NULL;
	pipe(fd);
	while (1)
	{
		buffer = readline(G "> " C);
		if (strcmp(buffer, path) == 0)
			break ;
		if (!text)
			text = ft_strdup(buffer);
		else
		{
			temp = ft_strjoin(text, buffer);
			free(text);
			text = temp;
		}
		text = ft_strjoin(text, "\n");
		free(buffer);
	}
	return (text);
}

void	heredoc(char *path, t_data *data, int redirection, t_ASTNode *s)
{
	int		fd[2];
	char	*text;

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
		execute_redirection(s->right, s->right->type, s->right->data, data);
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
		return (perror("open"), exit(1));
	if (s->right && s->right->type != CHAR_PIPE && s->right->type != 4)
	{
		if (s->right->type != redirection)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		execute_redirection(s->right, s->right->type, s->right->data, data);
		return ;
	}
	if (s->right && s->right->type == 4)
	{
		data->pipefd[1] = fd;
		execute_redirection(s->right, s->right->type, s->right->data, data);
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
			execute_redirection(s->right, s->right->type, s->right->data, data);
		ft_putstr_fd(M "-stanshell" C ": " Y, 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(C ": No such file or directory\n", 2);
		ast_destroy(data->ast_root);
		exit(1);
	}
	else if (s->right && s->right->type != CHAR_PIPE)
	{
		if (s->right->type != redirection && !check_heredoc(s->right))
			dup2(fd, STDIN_FILENO);
		close(fd);
		execute_redirection(s->right, s->right->type, s->right->data, data);
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	execute_redirection(t_ASTNode *save, int red, char *path, t_data *data)
{
	if (data->r_break && red != 4)
	{
		if (save->right && check_heredoc(save->right))
			return (execute_redirection(save->right, save->right->type, \
			save->right->data, data));
		else
			return ;
	}
	if (red == 3 || red == CHAR_OUTPUTR)
		output(path, data, red, save);
	else if (red == CHAR_INPUTR)
		input(path, data, red, save);
	else if (red == 4)
		heredoc(path, data, red, save);
}
