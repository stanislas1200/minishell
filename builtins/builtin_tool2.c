/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tool2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 00:07:18 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 23:19:59 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_identifier(char *var)
{
	int	i;

	i = -1;
	if (ft_strlen(var) <= 1)
		return (1);
	while (var[++i] && var[i] != '=')
	{
		if (i == 0 && !ft_isalpha(var[i]) && var[i] != '_')
			return (1);
		if (!ft_isalnum(var[i]) && var[i] != '_' && var[i] != '+')
			return (1);
		if (var[i] == '+' && var[i + 1] != '=')
			return (1);
	}
	if (var[i] != '=')
		return (2);
	return (0);
}

void	check_identifier(char **envp, char *var, int *i, int *append)
{
	int	j;
	int	ret;

	j = -1;
	ret = valid_identifier(var);
	if (ret == 1)
	{
		cperror("export", var, 0);
		ft_putstr_fd(": not a valid identifier\n", 2);
	}
	else if (ret == 0)
	{
		while (var[++j] && var[j] != '=')
			if (var[j] == '+')
				*append = 1;
		*i = j;
	}
}

char	*plus_remover(char *str)
{
	int		i;
	int		j;
	int		flag;
	char	*new_str;

	i = -1;
	j = 0;
	flag = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
	{
		cperror("export", "malloc", 1);
		return (NULL);
	}
	while (str[++i])
	{
		if (str[i] == '+' && !flag)
			flag = 1;
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	return (new_str);
}

void	update_old_pwd(char ***envp, char *path, int update)
{
	char	*tmp;

	if (update)
	{
		tmp = ft_strjoin("OLDPWD=", ft_getenv(*envp, "PWD"));
		if (!tmp)
			return ;
		export2(envp, tmp, 6, 0);
		free(tmp);
		update_pwd(envp);
	}
	else if (update == 0 && path && ft_strncmp(path, "-", 2) == 0)
	{
		if (chdir(ft_getenv(*envp, "OLDPWD")) == -1)
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
		update_old_pwd(envp, ft_getenv(*envp, "PWD"), 1);
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
