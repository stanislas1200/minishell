/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:41:45 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/04 16:52:03 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*free_join(char *s1, char *s2)
{
	char	*temp;

	if (s1)
	{
		temp = ft_strjoin(s1, s2);
		free (s1);
		s1 = NULL;
		return (temp);
	}
	return (ft_strdup(s2));
}

void	*parser_error(t_data *data)
{
	perror(R "Error: " C "malloc" C);
	data->parse_end = 1;
	return (NULL);
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

void	clean(t_data *data)
{
	free_matrix(data->env);
	ast_destroy(data->ast_root);
	rl_clear_history();
}

int	get_next_token(t_token **token)
{
	char	*new;

	if ((*token)->type == CHAR_INPUTR && (*token)->next \
		&& (*token)->next->type == CHAR_INPUTR)
	{
		*token = (*token)->next->next;
		if (token && (*token)->type != CHAR_NULL)
		{
			new = malloc(sizeof(char) * ft_strlen((*token)->data) + 1);
			if (!new)
				return (1);
			strip_quotes((*token)->data, new);
			free((*token)->data);
			(*token)->data = new;
			(*token) = (*token)->next;
			return (0);
		}
	}
	(*token) = (*token)->next;
	return (0);
}
