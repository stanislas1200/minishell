/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 23:21:27 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(char **args)
{
	int	i;
	int	flag;

	flag = 0;
	i = -1;
	while (args[++i])
	{
		if (i == 0 && ft_strncmp(args[i], "-n", 3) == 0)
			flag = 1;
		else
		{
			if ((i > 0 && !flag) || (i != 1 && flag))
				printf(" ");
			printf("%s", args[i]);
		}
	}
	if (!flag)
		printf("\n");
	free_matrix(args);
}

void	cd(char ***envp, char **paths)
{
	int	update;

	update = 0;
	if (matrix_len(paths) > 1)
		ft_putstr_fd("cd: too many arguments\n", 2);
	if (paths[0] && ft_strncmp(paths[0], "~", 2) \
	&& ft_strncmp(paths[0], "-", 2))
	{
		if (chdir(paths[0]) == -1)
			cperror("cd", paths[0], 1);
		else
			update = 1;
	}
	else if (!paths[0] || ft_strncmp(paths[0], "~", 2) == 0 \
	&& ft_strncmp(paths[0], "-", 2))
	{
		if (chdir(ft_getenv(*envp, "HOME")) == -1)
			ft_putstr_fd("cd: HOME not set\n", 2);
		else
			update = 1;
	}
	update_old_pwd(envp, paths[0], update);
	free_matrix(paths);
}

void	pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		cperror("getcwd", NULL, 1);
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
