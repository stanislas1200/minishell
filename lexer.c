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
#define QUOTE 1
#define DQUOTE 2

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
	if (token != NULL)
	{
		free(token->data);
		token_destroy(token->next);
		free(token);
	}
}

/*
** Lexer Build Function
**
** 1. Check if input string is valid
** 2. Create a new lexer
** 3. Create a new token
** 4. Initialize token properties
** 5. Add token to lexer
** 6. Return lexer
*/
t_lexer	*lexer_build(char *str)
{
	t_lexer	*lexer;		// 2. Create a new lexer
	t_token	*token;		// 3. Create a new token
	int		i;			// Index for iterating through input string
	int		j;			// Index for copying data to token
	int		size;	// Size of input string
	int		state;		// State of the lexer
	
	// 1. Check if input string is valid
	if (!str)
		return (NULL);
	size = strlen(str);
	
	// 2. Create a new lexer
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->tokens = malloc(sizeof(t_token));
	if (!lexer->tokens)
		return (free(lexer), NULL);
	
	// 3. Create a new token
	token = lexer->tokens;
	
	// 4. Initialize token properties
	if (token_init(token, size))
		return (free(lexer->tokens), free(lexer), NULL);
	
	i = -1;
	j = 0;
	
	state = GENERAL;
	while (str[++i])
	{	
		// Handle different character types
		if (state == GENERAL)
		{
			if (str[i] == CHAR_SPACE)
			{
				// End current token and start a new one
				if (j > 0) // If there is a token to end
				{
					token->data[j] = 0;
					token->next = malloc(sizeof(t_token));
					if (!token->next)
						return (token_destroy(lexer->tokens), free(lexer), NULL);
					token = token->next;
					if (token_init(token, size - i))
						return (token_destroy(lexer->tokens), free(lexer), NULL);
					j = 0;
				}
			}
			else if (str[i] == CHAR_QUOTE)
			{
				token->type = TOKEN;
				token->data[j++] = CHAR_QUOTE;
				state = QUOTE;
			}
			else if (str[i] == CHAR_DQUOTE)
			{
				token->type = TOKEN;
				token->data[j++] = CHAR_DQUOTE;
				state = DQUOTE;
			}
			else if (str[i] == CHAR_PIPE || str[i] == CHAR_INPUTR || str[i] == CHAR_OUTPUTR)
			{
				// End current token and start a new one
				if (j > 0) // If there is a token to end
				{
					token->data[j] = 0;
					token->next = malloc(sizeof(t_token));
					if (!token->next)
						return (token_destroy(lexer->tokens), free(lexer), NULL);
					token = token->next;
					if (token_init(token, size - i))
						return (token_destroy(lexer->tokens), free(lexer), NULL);
					j = 0;
				}

				// Pipe token
				token->data[0] = str[i];
				token->data[1] = 0;
				token->type = str[i];
				// Next token
				token->next = malloc(sizeof(t_token));
				if (!token->next)
					return (token_destroy(lexer->tokens), free(lexer), NULL);
				token = token->next;
				if (token_init(token, size - i))
					return (token_destroy(lexer->tokens), free(lexer), NULL);
			}
			else
			{
				// Copy character to token
				token->data[j++] = str[i];
				token->type = TOKEN;
			}
		}
		else if (state == QUOTE)
		{
				token->data[j++] = str[i];
				if (str[i] == CHAR_QUOTE)
					state = GENERAL;
		}
		else if (state == DQUOTE)
		{
				token->data[j++] = str[i];
				if (str[i] == CHAR_DQUOTE)
					state = GENERAL;
		}
	}
	token->data[j] = 0;

	// 6. Return the lexer
	return (lexer);
}

/* DEBUG */
void	lexer_print(t_lexer *lexer)
{
	t_token	*token;

	token = lexer->tokens;
	while (token)
	{
		printf("type: %d, value: %s\n", token->type, token->data);
		token = token->next;
	}
}
