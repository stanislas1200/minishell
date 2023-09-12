/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:24:17 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/12 17:52:30 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		cd2(char ***envp, char **paths);
void	update_pwd(char ***envp);
void	update_old_pwd(char ***envp);
long	ft_long_atoi(const char *nptr);

int	cd(char ***envp, char **paths)
{
	int	state;

	state = 0;
	if (paths[1] && ft_strncmp(paths[1], "-", 2) == 0)
	{
		if (chdir(ft_getenv(*envp, "OLDPWD")) == -1)
		{
			state = 1;
			ft_putstr_fd(R "cd: OLDPWD not set\n" C, 2);
		}
		else
			update_old_pwd(envp);
	}
	else
		state = cd2(envp, paths);
	return (state);
}

int	cd2(char ***envp, char **paths)
{
	int	state;

	state = 0;
	if (!paths[1])
	{
		if (chdir(ft_getenv(*envp, "HOME")) == -1)
		{
			state = 1;
			cperror("cd", NULL, "HOME not set", 0);
		}
		else
			update_old_pwd(envp);
	}
	else if (paths[1])
	{
		if (chdir(paths[1]) == -1)
		{
			state = 1;
			cperror("cd", paths[1], NULL, 1);
		}
		else
			update_old_pwd(envp);
	}
	return (state);
}

void	update_old_pwd(char ***envp)
{
	char	*tmp;

	tmp = ft_strjoin("OLDPWD=", ft_getenv(*envp, "PWD"));
	if (!tmp)
		return ;
	export2(envp, tmp, 6, 0);
	update_pwd(envp);
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
	}
}
