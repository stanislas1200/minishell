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

int	token_init(t_token *token, int size)
{
	token->data = malloc(size + 1);
	if (!token->data)
		return (1);
	token->data[0] = 0;
	token->type = CHAR_NULL;
	token->next = NULL;
	return (0);
}

void	token_destroy(t_token *token)
{
	if (!token)
		return ;
	if (token->data)
		free(token->data);
	token->data = NULL;
	token_destroy(token->next);
	free(token);
	token = NULL;
}

void	lexer_destroy(t_lexer *lexer)
{
	if (!lexer)
		return ;
	token_destroy(lexer->tokens);
	free(lexer);
	lexer = NULL;
}

void	*malloc_error(t_lexer *lexer)
{
	perror(R "Error: " C "malloc" C);
	lexer_destroy(lexer);
	return (NULL);
}

void	strip_quotes(char *src, char *dest)
{
	int	i;
	int	j;
	int	quote;

	i = -1;
	j = 0;
	quote = 0;
	while (src[++i])
	{
		if ((src[i] == '\'' || src[i] == '\"') && quote == 0)
			quote = src[i];
		else if (src[i] == quote)
			quote = 0;
		else
			dest[j++] = src[i];
	}
	dest[j] = '\0';
}

char	*expand_variables(char *input, t_data *data)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		var_start;
	char	*tmp;
	char	*tmp2;
	int		quote = 0;

	result = malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"') && quote == 0)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		if (input[i] == '$' && quote != '\'')
		{
			i++;
			var_start = i;
			while (ft_isalnum(input[i]))
				i++;
			if (input[var_start] == '?')
			{
				tmp = result;
				tmp2 = ft_itoa(data->last_exit);
				if (tmp2)
					result = ft_strjoin(result, tmp2);
				free(tmp2);
				free(tmp);
				if (!result || !tmp2)
					return (NULL);
				i++;
			}
			else if (i > var_start)
			{
				var_name = malloc(i - var_start + 1);
				if (!var_name)
					return (free(result), NULL);
				ft_strlcpy(var_name, input + var_start, i - var_start + 1);
				var_name[i - var_start] = '\0';
				var_value = ft_getenv(data->env, var_name);
				if (var_value)
				{
					tmp = result;
					result = ft_strjoin(result, var_value);
					free(tmp);
					if (!result)
						return (NULL);
				}
				free(var_name);
			}
			else
			{
				tmp = result;
				result = ft_strjoin(result, "$");
				free(tmp);
				if (!result)
					return (NULL);
			}
		}
		else
		{
			tmp = ft_calloc(ft_strlen(result) + 2, sizeof(char));
			if (!tmp)
				return (free(result), NULL);
			ft_strlcat(tmp, result, ft_strlen(result) + 2);
			ft_strlcat(tmp, input + i, ft_strlen(result) + 2);
			free(result);
			result = tmp;
			i++;
		}
	}
	return (result);
}

void	lexer_edit(t_lexer *lexer, t_data *data)
{
	t_token	*token;
	char	*var;
	char	*new;

	token = lexer->tokens;
	while (token)
	{
		new = expand_variables(token->data, data);
		if (!new)
			return ((void)malloc_error(lexer));
		if (new[0] == '\0')
			token->type = CHAR_NULL;
		free(token->data);
		token->data = new;
		if (token->type != CHAR_NULL)
		{
			new = malloc(sizeof(char) * ft_strlen(token->data) + 1);
			if (!new)
				return ((void)malloc_error(lexer));
			strip_quotes(token->data, new);
			free(token->data);
			token->data = new;
		}
		token = token->next;
	}
}

t_token	*token_new_next(t_token *token, t_data *data, int size)
{
	token->data[data->j] = 0;
	token->next = malloc(sizeof(t_token));
	if (!token->next)
		return (NULL);
	token = token->next;
	if (token_init(token, size - data->i))
		return (NULL);
	data->j = 0;
	return (token);
}

t_token	*lexer_general_next(t_token *token, t_data *data, char c, int size)
{
	if (c == CHAR_PIPE || c == CHAR_INPUTR || c == CHAR_OUTPUTR)
	{
		if (data->j > 0)
			token = token_new_next(token, data, size);
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
	while (str[++data->i] && token)
	{
		if (data->state == GENERAL)
			token = lexer_general(token, str[data->i], size, data);
		else if (data->state == CHAR_QUOTE || data->state == CHAR_DQUOTE)
		{
			token->data[data->j++] = str[data->i];
			if (str[data->i] == data->state)
				data->state = GENERAL;
		}
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
		return (malloc_error(lexer));
	lexer->tokens = malloc(sizeof(t_token));
	if (!lexer->tokens)
		return (malloc_error(lexer));
	token = lexer->tokens;
	if (token_init(token, size))
		return (malloc_error(lexer));
	if (lexer_lex(token, str, size, data))
		return (malloc_error(lexer));
	lexer_edit(lexer, data);
	return (lexer);
}
