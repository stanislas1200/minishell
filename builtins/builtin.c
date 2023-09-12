/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/12 14:36:12 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		cperror("getcwd", NULL, NULL, 1);
	return (0);
}

int	env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		printf("%s\n", envp[i++]);
	printf("_=/usr/bin/env\n");
	return (0);
}

int	is_builtin(t_ASTNode *node)
{
	if (ft_strncmp(node->data, "pwd", ft_strlen("pwd") + 1) == 0)
		return (1);
	if (ft_strncmp(node->data, "cd", ft_strlen("cd") + 1) == 0)
		return (1);
	if (ft_strncmp(node->data, "echo", ft_strlen("echo") + 1) == 0)
		return (1);
	if (ft_strncmp(node->data, "env", ft_strlen("env") + 1) == 0)
		return (1);
	if (ft_strncmp(node->data, "export", ft_strlen("export") + 1) == 0)
		return (1);
	if (ft_strncmp(node->data, "unset", ft_strlen("unset") + 1) == 0)
		return (1);
	if (ft_strncmp(node->data, "exit", ft_strlen("exit") + 1) == 0)
		return (1);
	return (0);
}
