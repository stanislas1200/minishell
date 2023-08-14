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

int	get_char_type(char c)
{
	if (c == 0)
		return CHAR_NULL;
	else if (c == '|')
		return CHAR_PIPE;
	else if (c == ' ')
		return CHAR_SPACE;
	else
		return CHAR_CHAR;
}

void	token_init(t_token *token, int size)
{
	token->data = malloc(size + 1);
	token->data[0] = 0;
	token->type = CHAR_NULL;
	token->next = NULL;
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
	int		chartype;	// Variable to store the type of character
	int		size;	// Size of input string
	
	// 1. Check if input string is valid
	if (!str)
		return (NULL);
	size = strlen(str);
	
	// 2. Create a new lexer
	lexer = malloc(sizeof(t_lexer));
	lexer->tokens = malloc(sizeof(t_token));
	
	// 3. Create a new token
	token = lexer->tokens;
	
	// 4. Initialize token properties
	token_init(token, size);
	
	i = -1;
	j = 0;
	
	while (str[++i])
	{
		// Get the type of character
		chartype = get_char_type(str[i]);
		
		// Handle different character types
		if (chartype == CHAR_CHAR)
		{
			// Copy character to token
			token->data[j++] = str[i];
			token->type = TOKEN;
		}
		else if (chartype == CHAR_SPACE)
		{
			// End current token and start a new one
			if (j > 0) // If there is a token to end
			{
				token->data[j] = 0;
				token->next = malloc(sizeof(t_token));
				token = token->next;
				token_init(token, size - i);
				j = 0;
			}
		}
		else if (chartype == CHAR_PIPE)
		{
			// End current token and start a new one
			if (j > 0) // If there is a token to end
			{
				token->data[j] = 0;
				token->next = malloc(sizeof(t_token));
				token = token->next;
				token_init(token, size - i);
				j = 0;
			}
			
			// Pipe token
			token->data[0] = chartype;
			token->data[1] = 0;
			token->type = CHAR_PIPE;
			// Next token
			token->next = malloc(sizeof(t_token));
			token = token->next;
			token_init(token, size - i);
			continue;
		}
	}
	token->data[j] = 0;
	
	// 6. Return the lexer
	return lexer;
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