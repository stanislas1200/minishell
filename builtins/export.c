/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:28:55 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/20 16:44:37 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_no_arg(char **envp);
char	*plus_remover(char *str);
void	export2(char ***envp, char *var, int i, int append);
void	add_to_env(char ***envp, char *str);
int		valid_identifier(char *var);
void	check_identifier(char **envp, char *var, int *i, int *append);

int	export(char ***envp, char **args)
{
	int		i;
	int		j;
	int		append;
	int		exit_code;

	j = 0;
	exit_code = 0;
	while (args[++j])
	{
		i = -1;
		append = 0;
		check_identifier(*envp, args[j], &i, &append);
		if (i > -1)
			export2(envp, args[j], i, append);
		else if (i == -1)
			exit_code = 1;
	}
	if (j == 1)
		export_no_arg(*envp);
	free_matrix(args);
	return(exit_code);
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

void	add_to_env(char ***envp, char *str)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (matrix_len(*envp) + 2));
	if (!new_env)
		return ;
	i = -1;
	while ((*envp)[++i])
	{
		new_env[i] = ft_strdup((*envp)[i]);
		if (!new_env[i])
		{
			free_matrix(new_env);
			return ;
		}
	}
	new_env[i] = ft_strdup(str);
	if (!new_env[i])
	{
		free_matrix(new_env);
		return ;
	}
	new_env[i + 1] = NULL;
	free_matrix(*envp);
	*envp = new_env;
}

void	print_sort_env(char **envp)
{
	int		i;
	int		j;
	int		len;
	char	*tmp;

	i = -1;
	while (envp && envp[++i])
	{
		j = -1;
		while (envp[++j + 1])
		{
			len = 0;
			while (envp[j][len] != '=' && envp[j + 1][len] != '=')
				len++;
			if (ft_strncmp(envp[j], envp[j + 1], len + 1) > 0)
			{
				tmp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = tmp;
			}
		}
	}
	i = -1;
	while (envp && envp[++i])
		printf("declare -x %s\n", envp[i]);
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
		cperror("export", "malloc", NULL, 1);
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
