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

void	*print_error(char *str, t_data *data)
{
	ft_putstr_fd(M "-stanshell: " C "syntax error near unexpected token `", 2);
	if (str)
	{
		if (str[0] == '\n')
			ft_putstr_fd("newline", 2);
		else if (str[0] == '\0')
			ft_putstr_fd("end of file", 2);
		else
			ft_putstr_fd(str, 2);
	}
	ft_putstr_fd("'\n", 2);
	data->parse_end = 1;
	data->last_exit = 2;
	return (NULL);
}

t_ASTNode	*command_simple(t_token **token, int type)
{
	t_ASTNode	*node;

	while (*token && (*token)->type == 0)
		*token = (*token)->next;
	if (!*token || (*token)->type != TOKEN)
		return (NULL);
	node = new_node(type, (*token)->data);
	if (!node)
		return (NULL); // MALLOC ERROR
	if (*token && (*token)->type == TOKEN)
		*token = (*token)->next;
	node->right = command_simple(token, ARG);
	return (node);
}

t_ASTNode	*job_pipe(t_token **token, t_data *data, t_ASTNode *left)
{
	t_ASTNode	*node;

	if (!left)
		left = command_simple(token, TOKEN);
	if (!left)
		return (NULL);
	if ((*token) && (*token)->type == CHAR_PIPE)
	{
		node = new_node(CHAR_PIPE, (*token)->data);
		if (!node)
			return (free_node(left));
		node->left = left;
		*token = (*token)->next;
		if ((*token)->type != TOKEN)
		{
			if (!(*token)->next || (*token)->next->type != TOKEN)
			{
				if (!(*token)->next || !(*token)->next->next || (*token)->next->type != (*token)->type)
				{
					data->parse_end = 1;
					return (print_error((*token)->data, data), free_node(left));
				}
			}
		}
		node->right = parse_top(*token, data);
		return (node);
	}
	return (free_node(left));
}

t_ASTNode	*redirection(t_token **token, t_data *data)
{
	int			type;
	int			old;
	t_ASTNode	*left;
	t_ASTNode	*node;
	t_token		*save;
	t_ASTNode	*new_root;
	t_token		*temp;

	left = NULL;
	if ((*token) && (*token)->type != CHAR_PIPE && (*token)->type != TOKEN && (*token)->next)
	{
		save = *token;
		while (save && save->type != CHAR_PIPE)
		{
			old = save->type;
			temp = save;
			save = save->next;
			while (save && save->type == 0)
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
	if (!(*token) || (*token)->type == CHAR_PIPE || (*token)->type == TOKEN || (*token)->type == 0)
		return (free_node(left));
	type = (*token)->type;
	if ((*token)->next && (*token)->next->type == type)
	{
		(*token) = (*token)->next;
		if (type == CHAR_INPUTR)
			type = 4;
		else if (type == CHAR_OUTPUTR)
			type = 3;
	}
	*token = (*token)->next;
	if (!(*token) || (*token)->type != TOKEN || (*token)->data[0] == '\n')
	{
		data->parse_end = 1;
		return (print_error((*token)->data, data), free_node(left));
	}
	node = new_node(type, (*token)->data);
	if (!node)
		return (free_node(left));
	node->left = left;
	while ((*token)->next && (*token)->next->type == 0)
		(*token) = (*token)->next;
	if ((*token)->next && (*token)->next->type == CHAR_PIPE)
	{
		*token = (*token)->next;
		new_root = new_node('|', (*token)->data);
		node->right = new_root;
		new_root->right = parse_top((*token)->next, data);
	}
	else
	{
		if ((*token)->next && (*token)->next->type != CHAR_PIPE && left)
		{
			while (left->right)
				left = left->right;
			save = *token;
			while (save && save->type != CHAR_PIPE)
			{
				old = save->type;
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

t_ASTNode	*parse_top(t_token *token, t_data *data)
{
	t_token		*save;
	t_ASTNode	*node;

	while (token && token->type == 0)
		token = token->next;
	if (!token || token->type == 0)
		return (NULL);
	node = NULL;
	save = token;
	node = job_pipe(&token, data, NULL);
	if (node && !data->parse_end)
		return (node);
	token = save;
	node = redirection(&token, data);
	if (node && !data->parse_end)
		return (node);
	token = save;
	node = command_simple(&token, TOKEN);
	if (node && !data->parse_end)
		return (node);
	if (data->parse_end)
		return (data->parse_end = 1, NULL);
	return (print_error(token->data, data));
}

t_ASTNode	*r(t_ASTNode **root,t_ASTNode *node, t_ASTNode *cmd, t_ASTNode *prev)
{
	t_ASTNode	*temp;

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
			temp = node;
			node = node->right;
			temp->right = NULL;
			free_node(temp);
		}
		else
		{
			prev = node;
			node = node->right;
		}
	}
	return (cmd);
}

void	check_eof(t_ASTNode **root)
{
	t_ASTNode	*node;
	t_ASTNode	*cmd;

	if (!*root)
		return ;
	node = *root;
	if (node->type == 4)
	{
		if (node->left)
			cmd = r(&(node->left), node->left, NULL, NULL);
		else
			cmd = NULL;
		if (!node->left)
			node->left = cmd;
		return ;
	}
	check_eof(&(node->right));
}

int	reorder_check_pipe(t_ASTNode **root, t_ASTNode *node, t_ASTNode *prev_save)
{
	t_ASTNode	*save;
	t_ASTNode	*prev;

	save = node;
	while (node && node->type != TOKEN && node->type != CHAR_PIPE)
	{
		prev = node;
		if (!node->right)
			return (1);
		node = node->right;
		if (node && (node->type == CHAR_PIPE))
		{
			prev->right = NULL;
			node->left = save;
			if (prev_save)
				prev_save->right = node;
			else
				*root = node;
			reorder_tree(&(node->right));
			return (1);
		}
	}
	return (0);
}

void	reorder_tree(t_ASTNode **root)
{
	t_ASTNode	*node;
	t_ASTNode	*prev_save;

	node = *root;
	prev_save = NULL;
	while (node && node->right)
	{
		if (node && (node->type == TOKEN || node->type == CHAR_PIPE))
			prev_save = node;
		if (node && node->type != TOKEN && node->type != CHAR_PIPE)
		{
			if (reorder_check_pipe(root, node, prev_save))
				return ;
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
		return (NULL);
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
