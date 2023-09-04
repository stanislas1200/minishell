/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 13:30:21 by sgodin            #+#    #+#             */
/*   Updated: 2023/09/01 13:47:25 by sgodin           ###   ########.fr       */
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

void	*ast_destroy(t_ASTNode *node)
{
	if (!node)
		return (NULL);
	ast_destroy(node->left);
	ast_destroy(node->right);
	if (node->data)
		free(node->data);
	node->data = NULL;
	free(node);
	node = NULL;
	return (NULL);
}

t_ASTNode	*command_simple(t_token **token, int type, t_data *data)
{
	t_ASTNode	*node;

	while (*token && (*token)->type == 0)
		*token = (*token)->next;
	if (!*token || (*token)->type != TOKEN)
		return (NULL);
	node = new_node(type, (*token)->data);
	if (!node)
		return (parser_error(data));
	if (*token && (*token)->type == TOKEN)
		*token = (*token)->next;
	node->right = command_simple(token, ARG, data);
	return (node);
}

int	check_arg(t_token **token, t_ASTNode *left)
{
	int			old;
	t_token		*save;

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
				if (!left->right)
					return (1);
				save->type = 0;
				left = left->right;
			}
		}
	}
	return (0);
}

int	check_cmd(t_token **token, t_ASTNode **left, t_data *data)
{
	t_token	*save;

	save = *token;
	if ((*token) && (*token)->type != CHAR_PIPE \
	&& (*token)->type != TOKEN && (*token)->next)
	{
		while (save && save->type != CHAR_PIPE)
		{
			data->state = save->type;
			save = save->next;
			while (save && save->type == 0)
				save = save->next;
			if (save && save->type == TOKEN && data->state == TOKEN)
			{
				*left = new_node(TOKEN, save->data);
				if (!(*left))
					return (1);
				save->type = 0;
				return (0);
			}
		}
	}
	else
		*left = command_simple(token, TOKEN, data);
	return (0);
}
