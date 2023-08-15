/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 21:32:40 by dspilleb         ###   ########.fr       */
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

void	cd(char **paths)
{
	if (matrix_len(paths) > 1)
	{
		ft_putstr_fd(R, 2);
		ft_putstr_fd("cd: too many arguments\n", 2);
	}
	if (paths[0] && chdir(paths[0]) == -1)
		cperror("cd", "path", 1);
	free_matrix(paths);
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

void	export(char ***envp, char **args)
{
	char	**tmp;
	int		i;
	int		j;
	int		append;

	j = -1;
	while (args[++j])
	{
		i = -1;
		append = 0;
		check_identifier(*envp, args[j], &i, &append);
		if (i > -1)
			export2(envp, args[j], i, append);
	}
	free_matrix(args);
}
