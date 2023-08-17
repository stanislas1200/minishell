/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:33:36 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/17 03:55:50 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char **env, char *str)
{
	int	i;

	i = -1;
	while (env[++i])
		if (strncmp(env[i], str, ft_strlen(str)) == 0)
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
	{
		cperror("Malloc", NULL, 1);
		return (-1);
	}
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
			return (i);
	return (-1);
}

char	**dup_env(char **envp)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (matrix_len(envp) + 1));
	if (!new_env)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			cperror("dup_env", "malloc", 1);
			free_matrix(new_env);
			return (NULL);
		}
	}
	new_env[i] = NULL;
	return (new_env);
}
