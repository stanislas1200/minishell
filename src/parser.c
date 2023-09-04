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

#include "../minishell.h"

t_ASTNode	*job_pipe(t_token **token, t_data *data, t_ASTNode *left)
{
	t_ASTNode	*node;

	if (!left)
		left = command_simple(token, TOKEN, data);
	if (!left)
		return (NULL);
	if ((*token) && (*token)->type == CHAR_PIPE)
	{
		node = new_node(CHAR_PIPE, (*token)->data);
		if (!node)
			return (ast_destroy(left), parser_error(data));
		node->left = left;
		*token = (*token)->next;
		if ((*token)->type != TOKEN)
		{
			if (!(*token)->next || (*token)->next->type != TOKEN)
			{
				if (!(*token)->next || !(*token)->next->next \
				|| (*token)->next->type != (*token)->type)
					return (print_error((*token)->data, data, node));
			}
		}
		return (node->right = parse_top(*token, data), node);
	}
	return (ast_destroy(left));
}

t_ASTNode	*r_n(t_token **token, t_data *data, t_ASTNode *left, int type)
{
	t_ASTNode	*node;
	t_ASTNode	*new_root;

	node = new_node(type, (*token)->data);
	if (!node)
		return (ast_destroy(left), parser_error(data));
	node->left = left;
	while ((*token)->next && (*token)->next->type == 0)
		(*token) = (*token)->next;
	if (check_arg(token, left))
		return (ast_destroy(node), parser_error(data));
	while ((*token)->next && (*token)->next->type == 0)
		(*token) = (*token)->next;
	if ((*token)->next && (*token)->next->type == CHAR_PIPE)
	{
		*token = (*token)->next;
		new_root = new_node('|', (*token)->data);
		if (!new_root)
			return (ast_destroy(node), parser_error(data));
		node->right = new_root;
		new_root->right = parse_top((*token)->next, data);
	}
	else
		node->right = parse_top((*token)->next, data);
	return (node);
}

t_ASTNode	*redirection(t_token **token, t_data *data)
{
	int			type;
	t_ASTNode	*left;

	left = NULL;
	if (check_cmd(token, &left, data))
		return (parser_error(data));
	if (!(*token) || (*token)->type == CHAR_PIPE \
	|| (*token)->type == TOKEN || (*token)->type == 0)
		return (ast_destroy(left));
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
		return (print_error((*token)->data, data, left));
	return (r_n(token, data, left, type));
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
	if (!data->parse_end)
		node = redirection(&token, data);
	if (node && !data->parse_end)
		return (node);
	token = save;
	if (!data->parse_end)
		node = command_simple(&token, TOKEN, data);
	if (node && !data->parse_end)
		return (node);
	if (data->parse_end)
		return (data->parse_end = 1, ast_destroy(node), NULL);
	return (print_error(token->data, data, NULL));
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
	return (tree);
}
