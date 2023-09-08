/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/08 16:23:51 by sgodin           ###   ########.fr       */
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
