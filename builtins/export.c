/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:28:55 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/13 13:01:10 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_no_arg(char **envp);
char	*plus_remover(char *str);
void	export2(char ***envp, char *var, int i, int append);
void	add_to_env(char ***envp, char *str);
int		valid_identifier(char *var);
char	*check_identifier(char **envp, char *var, int *i, int *append);
void	print_export_no_arg(char *str);
void	export3(char ***en, char *v, int i, int opt);

int	export(char ***envp, char **args)
{
	int		i;
	int		j;
	int		option;
	char	*tmp;

	j = 0;
	while (args[++j])
	{
		i = -1;
		option = NORMAL;
		tmp = check_identifier(*envp, args[j], &i, &option);
		printf("option %d\n", option);
		if (i != -1 && tmp)
			export2(envp, tmp, i, option);
		else if (i == -1)
			return (1);
	}
	if (j == 1)
		export_no_arg(*envp);
	return (0);
}

void	export2(char ***en, char *v, int i, int opt)
{
	char	*tmp;
	int		ret;

	if (!v)
		return ;
	ret = ft_getindexenv(*en, v);
	if (ret != -1 && !v[i])
		return (free(v));
	if (ret != -1 && opt == NORMAL || ret != -1 && opt == APPEND)
		export3(en, v, i, opt);
	else if (ret == -1 && opt == APPEND)
	{
		tmp = plus_remover(v);
		if (tmp)
			add_to_env(en, tmp);
		free(tmp);
	}
	else if (ret == -1)
		add_to_env(en, v);
	free(v);
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
		print_export_no_arg(envp[i]);
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
