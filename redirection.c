#include "minishell.h"

void    heredoc(char *path, t_data *data, int redirection, t_ASTNode *save)
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
	char *new;
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
	if (save->right && save->right->type != CHAR_PIPE)
	{
		if (save->right->type != redirection && save->right->type != CHAR_INPUTR)
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
		write(fd[1], text, strlen(text));
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
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
	if (redirection == CHAR_INPUTR)
	{
		int	fd;

		fd = open(path, O_RDONLY);
		if (fd == -1)
		{
			data->r_break = 1;
			if (save->right && check_heredoc(save->right))
				execute_redirection(save->right, save->right->type, save->right->data, data);
			ft_putstr_fd(M "-stanshell" C ": " Y, 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(C ": No such file or directory\n", 2);
			ast_destroy(data->ast_root);
			exit(1);
		}
		if (save->right && save->right->type != CHAR_PIPE)
		{
			if (save->right->type != redirection && !check_heredoc(save->right))
				dup2(fd, STDIN_FILENO);
			close(fd);
			execute_redirection(save->right, save->right->type, save->right->data, data);
			return ;
		}
		else
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (redirection == 4)
		heredoc(path, data, redirection, save);
}
