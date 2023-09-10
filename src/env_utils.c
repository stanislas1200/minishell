/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:33:36 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/10 13:41:43 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_numeric(char *str);

char	*ft_getenv(char **env, char *str)
{
	int	i;

	i = -1;
	while (env[++i])
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0)
			return (&env[i][ft_strlen(str) + 1]);
	return (NULL);
}

int	ft_getindexenv(char **env, char *str)
{
	int		i;
	int		len;
	char	*tmp;

	tmp = ft_strdup(str);
	if (!tmp)
		return (cperror("Malloc", NULL, NULL, 1), -1);
	len = 0;
	while (tmp[len] && (tmp[len] != '=' && tmp[len] != '+'))
		len++;
	if (tmp[len] == '+')
		tmp[len] = '=';
	if (tmp[len] == '=')
		len++;
	i = -1;
	while (env[++i])
		if (ft_strncmp(env[i], tmp, len) == 0)
			return (free(tmp), i);
	return (free(tmp), -1);
}

char	**dup_env(char **envp)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (matrix_len(envp) + 1));
	if (!new_env)
		return (cperror("dup_env", "malloc", NULL, 1), NULL);
	i = -1;
	while (envp[++i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			cperror("dup_env", "malloc", NULL, 1);
			free_matrix(new_env);
			return (NULL);
		}
	}
	new_env[i] = NULL;
	return (new_env);
}

void	shell_lvl(char ***envp)
{
	char		*tmp;
	int			num;
	char		*tmp2;

	errno = 0;
	num = 1;
	tmp2 = ft_getenv(*envp, "SHLVL");
	if (!tmp2)
		return (export2(envp, "SHLVL=1", 5, 0));
	if (is_numeric(tmp2) == 0)
	{
		num = ft_atoi(tmp2);
		if (num == INT_MAX || num == INT_MIN || num++ < 0 || errno != 0)
			num = 0;
	}
	tmp2 = ft_itoa(num);
	tmp = ft_strjoin("SHLVL=", tmp2);
	free(tmp2);
	if (!tmp)
		return (errno = 0, cperror("SHLVL", "malloc", NULL, 1));
	export2(envp, tmp, 5, 0);
	free(tmp);
	errno = 0;
}

void	update_env(char ***envp)
{
	int			i;
	char		*tmp;

	shell_lvl(envp);
	update_pwd(envp);
	i = ft_getindexenv(*envp, "_=");
	if (i != -1)
		delete_from_env(envp, i);
}
