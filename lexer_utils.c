/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 12:17:19 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/31 12:17:19 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_init(t_token *token, int size)
{
	token->type = CHAR_NULL;
	token->next = NULL;
	token->data = ft_calloc(size + 1, sizeof(char));
	if (!token->data)
		return (1);
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

void	*lexer_malloc_error(t_lexer *lexer)
{
	perror(R "Error: " C "malloc" C);
	if (lexer)
		lexer_destroy(lexer);
	return (NULL);
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
