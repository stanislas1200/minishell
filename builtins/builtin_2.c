/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:36:56 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 00:35:27 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**unset(char **envp, char *var)
{
	int		i;
	int		len;
	char	**ret;

	len = 0;
	i = ft_getindexenv(envp, var);
	if (i != -1)
	{
		while (envp[i][len] && envp[i][len] != '=')
			len++;
		if (ft_strncmp(envp[i], var, len) == 0 && ft_strlen(var) == len)
		{
			ret = delete_from_env(envp, i);
			if (ret)
				return (ret);
			else
				cperror("export", "malloc", 1);
		}
	}
	return (envp);
}

char	**export2(char **envp, char *var, int i, int append)
{
	char	*tmp;
	int		ret;

	ret = ft_getindexenv(envp, var);
	if (ret != -1 && append == 0)
	{
		tmp = ft_strdup(var);
		if (!tmp)
			return (NULL);
		free(envp[ret]);
		envp[ret] = tmp;
	}
	else if (ret != -1 && append == 1)
	{
		tmp = ft_strjoin(envp[ret], &var[++i]);
		if (!tmp)
			return (NULL);
		free(envp[ret]);
		envp[ret] = tmp;
	}
	else
		envp = add_to_env(envp, var);
	return (envp);
}
