/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 00:09:54 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(char *str, char option)
{
	if (str && option == 'n')
		printf("%s", str);
	if (str && option == 0)
		printf("%s\n", str);
	else if (option == 0)
		printf("\n");
}

void	cd(char *path)
{
	if (chdir(path) == -1)
		cperror("cd", "path", 1);
}

void	pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		("getcwd", NULL, 1);
}

void	env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		printf("%s\n", envp[i++]);
}

char	**export(char **envp, char *var)
{
	char	**tmp;
	int		i;
	int		append;

	i = -1;
	append = 0;
	if (valid_identifier(var) == 1)
	{
		cperror("export", var, 0);
		ft_putstr_fd("not a valid identifier\n", 2);
		return (envp);
	}
	while (var[++i] != '=')
		if (var[i] == '+')
			append = 1;
	tmp = export2(envp, var, i, append);
	if (tmp)
		return (tmp);
	else
	{
		cperror("export", "malloc", 1);
		return (envp);
	}
}
