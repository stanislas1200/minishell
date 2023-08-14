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

t_ASTNode *command_simple(t_token **token);
t_ASTNode *job_pipe(t_token **token);
t_ASTNode *parse_top(t_token *token);

t_ASTNode *command_simple(t_token **token) {
	t_ASTNode *node = malloc(sizeof(t_ASTNode));
	if (!node) {
		// Handle memory allocation failure
		return NULL;
	}

	node->type = TOKEN;
	node->data = strdup((*token)->data);
	node->left = NULL;
	node->right = NULL;

	// Move the token pointer to the next token
	*token = (*token)->next;

	// Process command arguments if available
	while (*token && (*token)->type != CHAR_PIPE) {
		t_ASTNode *arg_node = malloc(sizeof(t_ASTNode));
		if (!arg_node) {
			// Handle memory allocation failure
			return NULL;
		}

		arg_node->type = ARG;
		arg_node->data = strdup((*token)->data);
		arg_node->left = NULL;
		arg_node->right = NULL;

		// Append the argument node to the right of the command node
		if (!node->right) {
			node->right = arg_node;
		} else {
			t_ASTNode *temp = node->right;
			while (temp->right) {
				temp = temp->right;
			}
			temp->right = arg_node;
		}

		// Move the token pointer to the next token
		*token = (*token)->next;
	}

	return node;
}


t_ASTNode *job_pipe(t_token **token) {
	t_ASTNode *left = command_simple(token);
	if (!left) {
		return NULL;
	}

	if ((*token) && (*token)->type == CHAR_PIPE) {
		t_ASTNode *node = malloc(sizeof(t_ASTNode));
		if (!node) {
			// Handle memory allocation failure
			return NULL;
		}

		node->type = CHAR_PIPE;
		node->data = strdup((*token)->data);
		node->left = left;

		// Move the token pointer to the next token
		*token = (*token)->next;

		// Recursively parse the right side of the pipe
		node->right = job_pipe(token);

		return node;
	}

	return left;
}

t_ASTNode	*job_command(t_token *token)
{
	t_token *save = token;
	t_ASTNode *node = NULL;
	
	if ((token = save, node = command_simple(&token)) != NULL)
		return node;

	return (NULL);
}

t_ASTNode	*parse_top(t_token *token)
{
	t_token *save = token;
	t_ASTNode *node = NULL;
	
	if ((token = save, node = job_pipe(&token)) != NULL)
		return node;

	if ((token = save, node = job_command(token)) != NULL)
		return node;

	return (NULL);
}

t_ASTNode	*parse(t_lexer *lexer)
{
	t_ASTNode	*tree;
	t_token		*token;

	token = lexer->tokens;

	tree = parse_top(token);
	print_ast(tree);
	return (tree);
}


void print_ast_node(t_ASTNode *node, int indent)
{
	if (node == NULL)
		return;

	// Print the current node's data with indentation
	for (int i = 0; i < indent; i++)
		printf("    ");
	
	printf("Type: %d, Data: %s\n", node->type, node->data);

	// Recursively print left and right subtrees
	print_ast_node(node->left, indent + 1);
	print_ast_node(node->right, indent + 1);
}

void print_ast(t_ASTNode *root)
{
	printf("Abstract Syntax Tree:\n");
	print_ast_node(root, 0);
}
