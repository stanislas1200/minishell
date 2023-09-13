/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 00:07:18 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/13 13:54:22 by dspilleb         ###   ########.fr       */
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

char	*check_identifier(char **envp, char *var, int *i, int *option)
{
	int		j;
	int		ret;
	char	*tmp;

	j = -1;
	ret = valid_identifier(var);
	if (ret == 1)
		cperror("export", var, "not a valid identifier", 0);
	else if (ret == 0 | ret == 2)
	{
		while (var[++j] && var[j] != '=')
			if (var[j] == '+')
				*option = APPEND;
		*i = j;
		return (ft_strdup(var));
	}
	return (NULL);
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

void	print_export_no_arg(char *str)
{
	int	i;
	int	flag;

	i = -1;
	flag = 0;
	printf("declare -x ");
	while (str[++i])
	{
		printf("%c", str[i]);
		if (flag == 0 && str[i] == '=')
		{
			flag = 1;
			printf("\"");
		}
	}
	if (flag)
		printf("\"");
	printf("\n");
}
