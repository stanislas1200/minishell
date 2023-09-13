/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/13 18:31:33 by dspilleb         ###   ########.fr       */
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
	int	j;
	int	flag;

	flag = 0;
	i = 0;
	while (envp && envp[i])
	{
		flag = 0;
		j = -1;
		while (envp[i][++j])
			if (envp[i][j] == '=')
				flag = 1;
		if (flag)
			printf("%s\n", envp[i]);
		i++;
	}
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

char	*join_until_dollar(char *s1, char *input)
{
	char	*copy;
	int		i;

	i = 0;
	copy = ft_strdup(input);
	while (copy[i])
	{
		if (copy[i] == '$')
			copy[i] = 0;
		i++;
	}
	return (free_join(s1, copy));
}
