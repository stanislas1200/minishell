/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin_tool1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 00:05:29 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 00:08:36 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**delete_from_env(char **envp, int del)
{
	char	**new_env;
	int		i;
	int		j;

	i = matrix_len(envp);
	if (i < 1)
		return (envp);
	new_env = malloc(sizeof(char *) * (i));
	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (i != del)
		{
			new_env[j] = ft_strdup(envp[i]);
			if (!new_env[j++])
			{
				free_matrix(new_env);
				return (NULL);
			}
		}
	}
	new_env[j] = NULL;
	free_matrix(envp);
	return (new_env);
}

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
	int	i;
	int	len;

	len = 0;
	while (str[len] && (str[len] != '=' && str[len] != '+'))
		len++;
	i = -1;
	while (env[++i])
		if (strncmp(env[i], str, len - 1) == 0)
			return (i);
	return (-1);
}

char	**add_to_env(char **envp, char *str)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (matrix_len(envp) + 2));
	if (!new_env)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			free_matrix(new_env);
			return (NULL);
		}
	}
	new_env[i] = ft_strdup(str);
	if (!new_env[i])
	{
		free_matrix(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	free_matrix(envp);
	return (new_env);
}
