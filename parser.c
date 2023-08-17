/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:10:34 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/14 19:18:25 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ASTNode	*new_node(int t, char *d, char ***env)
{
	t_ASTNode	*node;

	node = malloc(sizeof(t_ASTNode));
	if (!node)
		return (NULL);
	node->type = t;
	node->data = ft_strdup(d);
	node->left = NULL;
	node->right = NULL;
	node->env = env;

	return (node);
}

t_ASTNode	*command_simple(t_token **token, char ***env, int type)
{
	t_ASTNode	*node;
	// Check token
	if (!*token || (*token)->type != TOKEN)
		return (NULL); // TODO

	// Init token
	node = new_node(type, (*token)->data, env);
	if (!node)
		return (NULL); // TODO

	// Move to the next
	if (*token && (*token)->type == TOKEN)
		*token = (*token)->next;
	// Get Arg if Arg
	node->right = command_simple(token, env, ARG);

	return (node);
}


t_ASTNode	*job_pipe(t_token **token, char ***env)
{
	t_ASTNode	*left;
	t_ASTNode	*node;

	left = command_simple(token, env, TOKEN);
	if (!left)
		return (NULL); // TODO

	if ((*token) && (*token)->type == CHAR_PIPE)
	{
		node = new_node(CHAR_PIPE, (*token)->data, env);
		if (!node)
			return (NULL); // TODO
		node->left = left;

		// Move the token pointer to the next token
		*token = (*token)->next;

		// Recursively parse the right side of the pipe
		node->right = parse_top(*token, env);

		return (node);
	}

	return (free(left), NULL);
}

t_ASTNode	*redirection(t_token **token, char ***env)
{
	// Check left side of the redirection operator
	t_ASTNode *left = command_simple(token, env, TOKEN);
	// Check if the token is a redirection operator
	if (!(*token) || ((*token)->type != CHAR_INPUTR && (*token)->type != CHAR_OUTPUTR))
		return (NULL);
	// Save the token type
	int type = (*token)->type;

	// Move the token pointer to the next token
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN)
		return (NULL);
	// Create a new node for the redirection operator
	t_ASTNode	*node = malloc(sizeof(t_ASTNode));
	if (!node) {
		// Handle memory allocation failure
		return (NULL);
	}

	// Set the node's data and type
	node->type = type;
	node->data = ft_strdup((*token)->data);
	node->left = left;
	// Recursively parse the right side of the pipe
	node->right = parse_top((*token)->next, env);
	node->env = env;

	return (node);
}

t_ASTNode	*job_command(t_token *token, char ***env)
{
	t_token *save = token;
	t_ASTNode *node = (NULL);

	if ((token = save, node = redirection(&token, env)) != NULL)		// <simple command> <|> <filename>
		return (node);
	if ((token = save, node = command_simple(&token, env, TOKEN)) != NULL)	// <simple command>
		return (node);

	return (NULL);
}

t_ASTNode	*redirection_append(t_token **token, char ***env)
{
	t_ASTNode	*left = command_simple(token, env, TOKEN);

	if ((*token) && (*token)->type == CHAR_OUTPUTR) {
		*token = (*token)->next;
		if (!(*token) || (*token)->type != CHAR_OUTPUTR)
			return NULL;
		*token = (*token)->next;
		if (!(*token))
			return NULL;
		t_ASTNode *node = malloc(sizeof(t_ASTNode));
		if (!node) {
			// Handle memory allocation failure
			return (NULL);
		}

		node->type = 3;
		node->data = ft_strdup((*token)->data);
		node->left = left;

		// Parse the right side of the append operator
		node->right = parse_top((*token)->next, env);
		node->env = env;

		return (node);
	}

	return (NULL);
}

t_ASTNode	*redirection_heredoc(t_token **token, char ***env)
{
	t_ASTNode	*left = command_simple(token, env, TOKEN);

	if ((*token) && (*token)->type == CHAR_INPUTR) {
		*token = (*token)->next;
		if (!(*token) || (*token)->type != CHAR_INPUTR)
			return NULL;
		*token = (*token)->next;
		if (!(*token))
			return NULL;
		t_ASTNode *node = malloc(sizeof(t_ASTNode));
		if (!node) {
			// Handle memory allocation failure
			return (NULL);
		}

		node->type = 4;
		node->data = ft_strdup((*token)->data);
		node->left = left;

		// Parse the right side of the append operator
		node->right = parse_top((*token)->next, env);
	node->env = env;

		return (node);
	}

	return (NULL);
}

t_ASTNode	*parse_top(t_token *token, char ***env)
{
	t_token		*save;
	t_ASTNode	*node;

	if (!token)
		return (NULL);
	save = token;
	node = NULL;

	if ((token = save, node = job_pipe(&token, env)) != NULL)	// <command> | <job>
		return (node);

	if ((token = save, node = redirection_append(&token, env)) != NULL) // <command> >> <filename>
		return (node);
	
	if ((token = save, node = redirection_heredoc(&token, env)) != NULL) // <command> << <filename>
		return (node);

	if ((token = save, node = job_command(token, env)) != NULL)	// <command>
		return (node);
	return (NULL);
}

t_ASTNode	*parse(t_lexer *lexer, char ***env)
{
	t_ASTNode	*tree;
	t_token		*token;

	token = lexer->tokens;

	tree = parse_top(token, env);
	return (tree);
}


void	print_ast_node(t_ASTNode *node, int indent)
{
	if (node == NULL)
		return ;

	// Print the current node's data with indentation
	for (int i = 0; i < indent; i++)
		printf("    ");

	printf("Type: %d, Data: %s\n", node->type, node->data);

	// Recursively print left and right subtrees
	print_ast_node(node->left, indent + 1);
	print_ast_node(node->right, indent + 1);
}

void print_ast_tree_vertical(t_ASTNode *node, int level) {
    if (node != NULL) {
        print_ast_tree_vertical(node->right, level + 1);

        for (int i = 0; i < level; i++) {
            printf("    ");
        }
		printf(M "|-- " C);
		if (node->type == CHAR_PIPE)
			printf(Y "%s" C, node->data);
		else if ( node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR | node->type == 3)
			printf(G "%c " C, node->type == 3 ? 'A' : node->type);
		else if (node->type == 4)
			printf(R "H " C);
        printf("%s\n", node->data);

        print_ast_tree_vertical(node->left, level + 1);
    }
}

void	print_ast(t_ASTNode *root)
{
	printf(Y "Abstract Syntax Tree:\n" C);
	print_ast_node(root, 0);
	printf("\n");
	print_ast_tree_vertical(root, 0);
}

/* CHECK  AFTER REDIRECTION */