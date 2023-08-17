/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:24:17 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/17 17:50:33 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd(char ***envp, char **paths)
{
	int	state;

	state = 0;
	if (matrix_len(paths) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		state = 1;
	}
	else if (paths[1] && ft_strncmp(paths[1], "-", 2) == 0)
	{
		if (chdir(ft_getenv(*envp, "OLDPWD")) == -1)
		{
			state = 1;
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
		}
		else
			update_old_pwd(envp, ft_getenv(*envp, "PWD"));
	}
	else
		state = cd2(envp, paths);
	free_matrix(paths);
	//exit(state);
}

int	cd2(char ***envp, char **paths)
{
	int	state;

	state = 0;

	if (!paths[1] || ft_strncmp(paths[1], "~", 2) == 0)
	{
		if (chdir(ft_getenv(*envp, "HOME")) == -1)
		{
			state = 1;
			ft_putstr_fd("cd: HOME not set\n", 2);
		}
		else
			update_old_pwd(envp, paths[1]);
	}
	else if (paths[1])
	{
		if (chdir(paths[1]) == -1)
		{
			state = 1;
			cperror("cd", paths[1], 1);
		}
		else
			update_old_pwd(envp, paths[1]);
	}
	return (state);
}

void	update_old_pwd(char ***envp, char *path)
{
	char	*tmp;

	if (path)
	{
		tmp = ft_strjoin("OLDPWD=", ft_getenv(*envp, "PWD"));
		if (!tmp)
			return ;
		export2(envp, tmp, 6, 0);
		free(tmp);
		update_pwd(envp);
	}
}

void	update_pwd(char ***envp)
{
	char	*tmp;
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		tmp = ft_strjoin("PWD=", cwd);
		if (!tmp)
			return ;
		export2(envp, tmp, 3, 0);
		free(tmp);
	}
}