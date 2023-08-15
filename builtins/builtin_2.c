/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:36:56 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 21:26:56 by dspilleb         ###   ########.fr       */
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
}

void	export2(char ***envp, char *var, int i, int append)
{
	char	*tmp;
	int		ret;

	ret = ft_getindexenv(*envp, var);
	if (ret != -1 && append == 0 || ret != -1 && append == 1)
	{
		if (ret != -1 && append == 0)
			tmp = ft_strdup(var);
		else
			tmp = ft_strjoin((*envp)[ret], &var[++i]);
		free((*envp)[ret]);
		(*envp)[ret] = tmp;
	}
	else if (ret == -1 && append == 1)
	{
		tmp = plus_remover(var);
		if (tmp)
			add_to_env(envp, tmp);
		free(tmp);
	}
	else
		add_to_env(envp, var);
}
