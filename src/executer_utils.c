/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 18:20:02 by sgodin            #+#    #+#             */
/*   Updated: 2023/09/04 18:23:46 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(t_ASTNode *node, char **arr, t_data *data)
{
	if (ft_strncmp(node->data, "pwd", ft_strlen("pwd") + 1) == 0)
		return (pwd());
	if (ft_strncmp(node->data, "cd", ft_strlen("cd") + 1) == 0)
		return (cd(&data->env, arr));
	if (ft_strncmp(node->data, "echo", ft_strlen("echo") + 1) == 0)
		return (echo(arr));
	if (ft_strncmp(node->data, "env", ft_strlen("env") + 1) == 0)
		return (env(data->env));
	if (ft_strncmp(node->data, "export", ft_strlen("export") + 1) == 0)
		return (export(&data->env, arr));
	if (ft_strncmp(node->data, "unset", ft_strlen("unset") + 1) == 0)
		return (unset(&data->env, arr));
	if (ft_strncmp(node->data, "exit", ft_strlen("exit") + 1) == 0)
		return (ft_exit(data->env, arr, data), 0);
	return (-1);
}

int	check_heredoc(t_ASTNode *node)
{
	t_ASTNode	*temp;

	temp = node;
	while (temp)
	{
		if (temp->type == CHAR_PIPE)
			if (check_heredoc(temp->left))
				return (1);
		if (temp->type == 4)
			return (1);
		temp = temp->right;
	}
	return (0);
}

char	**make_cmd_arr(t_ASTNode *node, t_ASTNode *arg_node)
{
	int			i;
	int			num_args;
	char		**arr;

	arr = NULL;
	num_args = 0;
	while (arg_node)
	{
		num_args++;
		arg_node = arg_node->right;
	}
	arr = (char **)malloc((num_args + 2) * sizeof(char *));
	if (!arr)
		return (perror("malloc"), NULL);
	arr[0] = node->data;
	i = 1;
	arg_node = node->right;
	while (arg_node)
	{
		arr[i++] = arg_node->data;
		arg_node = arg_node->right;
	}
	arr[i] = NULL;
	return (arr);
}
