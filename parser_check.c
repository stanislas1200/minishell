/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 13:30:52 by sgodin            #+#    #+#             */
/*   Updated: 2023/09/01 13:45:34 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*print_error(char *str, t_data *data, t_ASTNode *node)
{
	if (node)
		ast_destroy(node);
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

t_ASTNode	*r(t_ASTNode **root, t_ASTNode *node, t_ASTNode *cmd, t_ASTNode *p)
{
	t_ASTNode	*temp;

	while (node)
	{
		if (node->type == CHAR_INPUTR)
		{
			if (node->left)
				cmd = node->left;
			if (p)
				p->right = node->right;
			else
				*root = node->right;
			temp = node;
			node = node->right;
			temp->right = NULL;
			ast_destroy(temp);
		}
		else
		{
			p = node;
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
	check_eof(root);
}
