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

t_ASTNode	*job_pipe(t_token **token, char ***env, t_ASTNode *left)
{
	// t_ASTNode	*left;
	t_ASTNode	*node;

	if (!left)
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
	int			type;
	t_ASTNode	*left;
	t_ASTNode	*node;

	// Check left side of the redirection operator
	left = command_simple(token, env, TOKEN);
	// Check if the token is a redirection operator
	if (!(*token) || ((*token)->type != CHAR_INPUTR && (*token)->type != CHAR_OUTPUTR))
		return (NULL);
	// Save the token type
	type = (*token)->type;

	// Move the token pointer to the next token
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN)
		return (NULL);
	// Create a new node for the redirection operator
	node = new_node(type, (*token)->data, env);
	if (!node)
		return (NULL);

	// Set the node's left
	node->left = left;
	// Recursively parse the right side of the pipe
	if ((*token)->next && (*token)->next->type == CHAR_PIPE)
	{
		printf(G "pipe\n");
		// node->right = parse_top((*token)->next, env);
		// *token = (*token)->next;
		// return (job_pipe(token, env, node));
		*token = (*token)->next;
		t_ASTNode	*new_root = new_node('|', (*token)->data, env);
		node->right = new_root;
		new_root->right = parse_top((*token)->next, env);
	}
	else
		node->right = parse_top((*token)->next, env);

	return (node);
}

t_ASTNode	*job_command(t_token *token, char ***env)
{
	t_token		*save;
	t_ASTNode	*node;

	save = token;
	node = NULL;

	if ((node = redirection(&token, env)) != NULL)		// <simple command> <|> <filename>
		return (node);
	token = save;
	if ((node = command_simple(&token, env, TOKEN)) != NULL)	// <simple command>
		return (node);

	return (NULL);
}

t_ASTNode	*redirection_append(t_token **token, char ***env)
{
	t_ASTNode	*left;
	t_ASTNode	*node;

	left = command_simple(token, env, TOKEN);
	if ((*token) && (*token)->type == CHAR_OUTPUTR)
	{
		*token = (*token)->next;
		if (!(*token) || (*token)->type != CHAR_OUTPUTR)
			return (NULL);
		*token = (*token)->next;
		if (!(*token))
			return (NULL);
		node = new_node(3, (*token)->data, env);
		if (!node)
			return (NULL);
		node->left = left;

	// Recursively parse the right side of the pipe
	if ((*token)->next && (*token)->next->type == CHAR_PIPE)
	{
		printf(G "pipe\n");
		// node->right = parse_top((*token)->next, env);
		// *token = (*token)->next;
		// return (job_pipe(token, env, node));
		*token = (*token)->next;
		t_ASTNode	*new_root = new_node('|', (*token)->data, env);
		node->right = new_root;
		new_root->right = parse_top((*token)->next, env);
	}
	else
		node->right = parse_top((*token)->next, env);

		// // Parse the right side of the append operator
		// node->right = parse_top((*token)->next, env);

		return (node);
	}

	return (NULL);
}

t_ASTNode	*redirection_heredoc(t_token **token, char ***env)
{
	t_ASTNode	*left;
	t_ASTNode	*node;

	left = command_simple(token, env, TOKEN);
	if ((*token) && (*token)->type == CHAR_INPUTR)
	{
		*token = (*token)->next;
		if (!(*token) || (*token)->type != CHAR_INPUTR)
			return (NULL);
		*token = (*token)->next;
		if (!(*token))
			return (NULL);
		node = new_node(4, (*token)->data, env);
		if (!node)
			return (NULL);

		node->left = left;

	// Recursively parse the right side of the pipe
	if ((*token)->next && (*token)->next->type == CHAR_PIPE)
	{
		printf(G "pipe\n");
		// node->right = parse_top((*token)->next, env);
		// *token = (*token)->next;
		// return (job_pipe(token, env, node));
		*token = (*token)->next;
		t_ASTNode	*new_root = new_node('|', (*token)->data, env);
		node->right = new_root;
		new_root->right = parse_top((*token)->next, env);
	}
	else
		node->right = parse_top((*token)->next, env);

		// // Parse the right side of the append operator
		// node->right = parse_top((*token)->next, env);

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

	if ((node = job_pipe(&token, env, NULL)) != NULL)	// <command> | <job>
		return (node);
	token = save;
	if ((node = redirection_append(&token, env)) != NULL) // <command> >> <filename>
		return (node);
	token = save;
	if ((node = redirection_heredoc(&token, env)) != NULL) // <command> << <filename>
		return (node);
	token = save;
	if ((node = job_command(token, env)) != NULL)	// <command>
		return (node);
	return (NULL);
}

void reorder_tree(t_ASTNode **root) // TODO : Handle all type of redirections && don't send data to the pipe
{
	t_ASTNode *node = *root;
	t_ASTNode *prev = NULL; // Keep track of the previous node
	t_ASTNode *prev_save = NULL; // Keep track of the previous node that is not a redirection
	t_ASTNode *save = NULL;
	// int			type = 0;
	
	if (node == NULL)
		return;
	
	while (node && node->right)
	{

		// if (node && !type && node->type != TOKEN && node->type != CHAR_PIPE)
		// 	type = node->type; // replace && node->type != TOKEN && node->type != CHAR_PIPE by type
		if (node && (node->type == TOKEN || node->type == CHAR_PIPE))
			prev_save = node;
		if (node && node->type != TOKEN && node->type != CHAR_PIPE)
		{
			save = node;
			while (node && node->type != TOKEN && node->type != CHAR_PIPE)
			{
				prev = node;
				if (!node->right)
					return ;
				node = node->right;
				if (node && node->type == CHAR_PIPE)
				{
					prev->right = NULL;
					node->left = save;
					if (prev_save)
						prev_save->right = node;
					else
						*root = node;
					return ;
				}
			}

		}
		node = node->right;
	}
}

t_ASTNode	*parse(t_lexer *lexer, char ***env)
{
	t_ASTNode	*tree;
	t_token		*token;

	token = lexer->tokens;

	tree = parse_top(token, env);
	print_ast_tree_vertical(tree, 0);
	reorder_tree(&tree);
	return (tree);
}

/* Debug */

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