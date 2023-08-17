/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:36:56 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/17 04:00:36 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset(char ***envp, char **args)
{
	int		i;
	int		j;
	int		len;
	char	**ret;

	j = -1;
	while (args[++j])
	{
		len = 0;
		i = ft_getindexenv(*envp, args[j]);
		if (i != -1)
		{
			while ((*envp)[i][len] && (*envp)[i][len] != '=')
				len++;
			if (ft_strncmp((*envp)[i], args[j], len) == 0 \
			&& ft_strlen(args[j]) == len)
				delete_from_env(envp, i);
		}
	}
	free_matrix(args);
	exit(0);
}

void	delete_from_env(char ***envp, int del)
{
	char	**new_env;
	int		i;
	int		j;

	i = matrix_len(*envp);
	new_env = malloc(sizeof(char *) * (i));
	if (!new_env)
		return ;
	i = -1;
	j = 0;
	while ((*envp)[++i])
	{
		if (i != del)
		{
			new_env[j] = ft_strdup((*envp)[i]);
			if (!new_env[j++])
			{
				free_matrix(new_env);
				return ;
			}
		}
	}
	new_env[j] = NULL;
	free_matrix(*envp);
	*envp = new_env;
}
