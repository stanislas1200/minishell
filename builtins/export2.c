/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 00:07:18 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/20 14:08:51 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_sort_env(char **envp);

int	valid_identifier(char *var)
{
	int	i;

	i = -1;
	if (ft_strlen(var) < 1)
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
	if (i == 0)
		return (1);
	if (!var[i])
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
		cperror("export", var, "not a valid identifier", 0);
	else if (ret == 0)
	{
		while (var[++j] && var[j] != '=')
			if (var[j] == '+')
				*append = 1;
		*i = j;
	}
	else
		*i = -2;
}

void	export_no_arg(char **envp)
{
	char	**tmp;

	tmp = dup_env(envp);
	if (!tmp)
		cperror("export", "malloc", NULL, 1);
	else
	{
		print_sort_env(tmp);
		free_matrix(tmp);
	}
}
