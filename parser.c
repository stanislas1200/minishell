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

t_ASTNode	*new_node(int t, char *d)
{
	t_ASTNode	*node;

	node = malloc(sizeof(t_ASTNode));
	if (!node)
		return (NULL);
	node->type = t;
	node->data = ft_strdup(d);
	if (!node->data)
		return (free(node), NULL);
	node->left = NULL;
	node->right = NULL;

	return (node);
}

void	*free_node(t_ASTNode *node)
{
	if (!node)
		return (NULL);
	if (node->left)
		free_node(node->left);
	if (node->right)
		free_node(node->right);
	if (node->data)
		free(node->data);
	node->data = NULL;
	free(node);
	node = NULL;
	return (NULL);
}

void	ast_destroy(t_ASTNode *node)
{
	if (!node)
		return ;
	ast_destroy(node->left);
	ast_destroy(node->right);
	if (node->data)
		free(node->data);
	node->data = NULL;
	free(node);
	node = NULL;
}

t_ASTNode	*command_simple(t_token **token, int type)
{
	t_ASTNode	*node;
	// Check token
	if (!*token || (*token)->type != TOKEN)
		return (NULL); // TODO

	// Init token
	node = new_node(type, (*token)->data);
	if (!node)
		return (NULL); // TODO

	// Move to the next
	if (*token && (*token)->type == TOKEN)
		*token = (*token)->next;
	// Get Arg if Arg
	node->right = command_simple(token, ARG);

	return (node);
}

t_ASTNode	*job_pipe(t_token **token, t_data *data, t_ASTNode *left)
{
	// t_ASTNode	*left;
	t_ASTNode	*node;

	if (!left)
		left = command_simple(token, TOKEN);
	if (!left)
		return (NULL); // TODO

	if ((*token) && (*token)->type == CHAR_PIPE)
	{
		node = new_node(CHAR_PIPE, (*token)->data);
		if (!node)
			return (free_node(left));
		node->left = left;

		// Move the token pointer to the next token
		*token = (*token)->next;

		// Recursively parse the right side of the pipe
		if ((*token)->type != TOKEN)
		{
		data->parse_end = 1;
			return (ft_putstr_fd(M "-stanshell: " C "syntax error near unexpected token `",2), ft_putstr_fd((*token)->data, 2), ft_putstr_fd("'\n", 2), data->last_exit = 2, free_node(left));
		}
		node->right = parse_top(*token, data);

		return (node);
	}

	return (free_node(left));
}

t_ASTNode	*redirection(t_token **token, t_data *data)
{
	int			type;
	t_ASTNode	*left = NULL;
	t_ASTNode	*node;

	// Check left side of the redirection operator
	if ((*token) && (*token)->type != CHAR_PIPE && (*token)->type != TOKEN && (*token)->next)
	{
		t_token *save = *token;
		while (save && save->type != CHAR_PIPE && save->type != 0)
		{
			int old = save->type; // BE SURE
			t_token *temp = save;
			save = save->next;
			if (save && save->type == TOKEN && old == TOKEN)
			{
				left = new_node(TOKEN, save->data);
				save->type = 0;
			}
		}
	}
	else
		left = command_simple(token, TOKEN);

	// Check if the token is a redirection operator
	if (!(*token) || (*token)->type == CHAR_PIPE || (*token)->type == TOKEN)
		return (free_node(left));
	// Save the token type
	type = (*token)->type;
	// Check double
	if ((*token)->next && (*token)->next->type == type)
	{
		(*token) = (*token)->next;
		if (type == CHAR_INPUTR)
			type = 4;
		else if (type == CHAR_OUTPUTR)
			type = 3;
	}
	// Move the token pointer to the next token (the filename)
	*token = (*token)->next;
	// Check if the token is a filename
	if (!(*token) || (*token)->type != TOKEN || (*token)->data[0] == '\n')
	{
		data->parse_end = 1;
		return (ft_putstr_fd(M "-stanshell: " C "syntax error near unexpected token `",2), ft_putstr_fd((*token)->data, 2), ft_putstr_fd("'\n", 2), data->last_exit = 2, free_node(left));
	}
	// Init the node
	node = new_node(type, (*token)->data);
	if (!node)
		return (free_node(left));

	// Set the node's left
	node->left = left;
	
	// Move the token pointer to the next token if arg
	while ((*token)->next && (*token)->next->type == 0)
		(*token) = (*token)->next;
	// Recursively parse the right side of the pipe
	if ((*token)->next && (*token)->next->type == CHAR_PIPE)
	{
		*token = (*token)->next;
		t_ASTNode	*new_root = new_node('|', (*token)->data);
		node->right = new_root;
		new_root->right = parse_top((*token)->next, data);
	}
	else
	{
		// Get all arg if multiple redirections
		if ((*token)->next && (*token)->next->type != CHAR_PIPE && left)
		{
			while (left->right)
				left = left->right;
			t_token *save = *token;
			while (save && save->type != CHAR_PIPE)
			{
				int old = save->type; // BE SURE
				save = save->next;
				if (save && save->type == TOKEN && (old == TOKEN || !old))
				{
					left->right = new_node(TOKEN, save->data);
					save->type = 0;
					left = left->right;
				}
			}
		}
		while ((*token)->next && (*token)->next->type == 0)
			(*token) = (*token)->next;
		node->right = parse_top((*token)->next, data);
	}
	return (node);
}

t_ASTNode	*job_command(t_token *token, t_data *data)
{
	t_token		*save;
	t_ASTNode	*node;

	save = token;
	node = NULL;

	if ((node = redirection(&token, data)) != NULL)		// <simple command> <|> <filename>
		return (node);
	token = save;
	if ((node = command_simple(&token, TOKEN)) != NULL)	// <simple command>
		return (node);

	return (NULL);
}

t_ASTNode	*parse_top(t_token *token, t_data *data)
{
	t_token		*save;
	t_ASTNode	*node;

	while (token && token->type == 0)
		token = token->next;

	if (!token || token->type == 0)
		return (NULL);
	save = token;
	node = NULL;
	
	if ((node = job_pipe(&token, data, NULL)) != NULL && !data->parse_end)	// <command> | <job>
		return (node);
	token = save;
	if ((node = job_command(token, data)) != NULL && !data->parse_end)	// <command>
		return (node);
	if (data->parse_end)
		return (data->parse_end = 1, NULL);
	return(ft_putstr_fd(M "-stanshell: " C "syntax error near unexpected token `",2), ft_putstr_fd(token->data, 2), ft_putstr_fd("'\n", 2),data->parse_end = 1, NULL);
}

t_ASTNode *remove_all_input_nodes(t_ASTNode **root)
{
	t_ASTNode *cmd = NULL;
	if (!*root)
		return (NULL);

	t_ASTNode *node = *root;
	t_ASTNode *prev = NULL;

	while (node)
	{
		if (node->type == CHAR_INPUTR)
		{
			if (node->left)
				cmd = node->left;
			if (prev)
				prev->right = node->right;
			else
				*root = node->right;

			t_ASTNode *temp = node;
			node = node->right;
			temp->right = NULL;
			free(temp->data);
			temp->data = NULL;
			free(temp);
			temp = NULL;
		}
		else
		{
			prev = node;
			node = node->right;
		}
	}
	return (cmd);
}

void check_eof(t_ASTNode **root)
{
	t_ASTNode	*node;
	t_ASTNode	*cmd;


	if (!*root)
		return;
	node = *root;
	if (node->type == 4)
	{
		cmd = remove_all_input_nodes(&(node->left));
		if (!node->left)
			node->left = cmd;
		return;
	}
	check_eof(&(node->right));
}

void reorder_tree(t_ASTNode **root)
{
	t_ASTNode *node;
	t_ASTNode *prev;
	t_ASTNode *prev_save;
	t_ASTNode *save;

	node = *root;
	prev_save = NULL;
	while (node && node->right)
	{
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
				if (node && (node->type == CHAR_PIPE))
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

t_ASTNode	*parse(t_lexer *lexer, t_data *data)
{
	t_ASTNode	*tree;
	t_token		*token;

	token = lexer->tokens;
	tree = parse_top(token, data);
	if (!tree)
		return (data->last_exit = 2, NULL);
	if (data->parse_end)
	{
		ast_destroy(tree);
		tree = NULL;
		data->parse_end = 0;
		return (data->last_exit = 2, NULL);
	}
	reorder_tree(&tree);
	check_eof(&tree);
	return (tree);
}
