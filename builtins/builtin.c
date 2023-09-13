/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/13 18:21:17 by dspilleb         ###   ########.fr       */
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
