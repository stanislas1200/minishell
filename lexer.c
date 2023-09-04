/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 11:42:43 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/14 14:13:32 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define GENERAL 0

int	lexer_edit(t_token *token, t_data *data)
{
	char	*var;
	char	*new;

	while (token)
	{
		new = expand_variables(token->data, data);
		if (!new)
			return (1);
		if (new[0] == '\0')
			token->type = CHAR_NULL;
		free(token->data);
		token->data = new;
		if (token->type != CHAR_NULL)
		{
			new = malloc(sizeof(char) * ft_strlen(token->data) + 1);
			if (!new)
				return (1);
			strip_quotes(token->data, new);
			free(token->data);
			token->data = new;
		}
		token = token->next;
	}
	return (0);
}

t_token	*lexer_general_next(t_token *token, t_data *data, char c, int size)
{
	if (c == CHAR_PIPE || c == CHAR_INPUTR || c == CHAR_OUTPUTR)
	{
		if (data->j > 0)
			token = token_new_next(token, data, size);
		if (!token)
			return (NULL);
		token->data[0] = c;
		token->data[1] = 0;
		token->type = c;
		token->next = malloc(sizeof(t_token));
		if (!token->next)
			return (NULL);
		token = token->next;
		if (token_init(token, size - data->i))
			return (NULL);
	}
	else
	{
		token->data[data->j++] = c;
		token->type = TOKEN;
	}
	return (token);
}

t_token	*lexer_general(t_token *token, char c, int size, t_data *data)
{
	if (c == CHAR_SPACE || c == CHAR_TAB || c == CHAR_NEWLINE)
	{
		if (data->j > 0)
			token = token_new_next(token, data, size);
	}
	else if (c == CHAR_QUOTE || c == CHAR_DQUOTE)
	{
		token->type = TOKEN;
		token->data[data->j++] = c;
		data->state = c;
	}
	else
		token = lexer_general_next(token, data, c, size);
	return (token);
}

int	lexer_lex(t_token *token, char *str, int size, t_data *data)
{
	data->state = GENERAL;
	data->i = -1;
	data->j = 0;
	while (str[++data->i])
	{
		if (data->state == GENERAL)
			token = lexer_general(token, str[data->i], size, data);
		else if (data->state == CHAR_QUOTE || data->state == CHAR_DQUOTE)
		{
			token->data[data->j++] = str[data->i];
			if (str[data->i] == data->state)
				data->state = GENERAL;
		}
		if (!token)
			return (1);
	}
	token->data[data->j] = 0;
	return (0);
}

t_lexer	*lexer_build(char *str, t_data *data)
{
	t_lexer	*lexer;
	t_token	*token;
	int		i;
	int		j;
	int		size;

	lexer = NULL;
	token = NULL;
	if (!str)
		return (NULL);
	size = ft_strlen(str);
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (lexer_malloc_error(lexer));
	lexer->tokens = malloc(sizeof(t_token));
	if (!lexer->tokens || token_init(lexer->tokens, size))
		return (lexer_malloc_error(lexer));
	if (lexer_lex(lexer->tokens, str, size, data) \
	|| lexer_edit(lexer->tokens, data))
		return (lexer_malloc_error(lexer));
	return (lexer);
}
