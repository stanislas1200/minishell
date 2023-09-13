/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:33:36 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/13 12:56:52 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		is_numeric(char *str);
char	*expand_home(t_data *data, char *input, int i);

char	*ft_getenv(char **env, char *str)
{
	int	i;

	i = -1;
	while (env[++i])
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0)
			if (env[i][ft_strlen(str)] == '=')
				return (&env[i][ft_strlen(str) + 1]);
	return (NULL);
}

int	ft_getindexenv(char **env, char *str)
{
	int		i;
	int		j;
	int		flag;

	i = -1;
	while (env[++i])
	{
		flag = 0;
		j = 0;
		while (env[i][j] && str[j])
		{
			if (str[j] != env[i][j])
				flag = 1;
			if (env[i][j] == '=' || str[j] == '+' || str[j] == '=')
				break ;
			j++;
		}
		if (!flag && (str[j] == env[i][j] || !str[j] || !env[i][j]))
			return (i);
	}
	return (-1);
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
		return (export2(envp, ft_strdup("SHLVL=1"), 5, 0));
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
	errno = 0;
}

void	update_env(char ***envp)
{
	int			i;
	char		*tmp;
	static int	count;

	if (count == 0)
	{
		shell_lvl(envp);
		update_pwd(envp);
		count++;
	}
	i = ft_getindexenv(*envp, "_=");
	if (i != -1)
		delete_from_env(envp, i);
}
