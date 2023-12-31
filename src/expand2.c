/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 17:40:13 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/14 13:55:03 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		expand(t_data *data, char **result, char *input, int var_start);
int		result_add(t_data *data, char **result, char *input, char *tmp);
int		valid_identifier(char *var);
char	*expand_home2(t_data *data, char *input, int i, char *tmp);

char	*get_home(char **env)
{
	char	*ret;

	ret = ft_getenv(env, "HOME");
	if (!ret)
		return (getenv("HOME"));
	return (ret);
}

int	ft_isspace(char n)
{
	return (n == ' ' || n == '\t' || n == '\n' || \
	n == '\v' || n == '\f' || n == '\r');
}

void	expand_home(t_data *data, char *input, int i, char **result)
{
	char	*save;

	save = ft_strdup(*result);
	free(*result);
	if (input[i + 1] == '+')
		*result = ft_getenv(data->env, "PWD");
	else if (input[i + 1] == '-')
		*result = ft_getenv(data->env, "OLDPWD");
	else 
		*result = get_home(data->env);
	if (*result)
		*result = ft_strdup(*result);
	if (input[i + 1] == '-' || input[i + 1] == '+')
		data->i++;
	data->i++;
	if (!*result)
		return ((void)(*result = save));
	if (*result && i > 0)
		*result = expand_home2(data, input, i, *result);
	free(save);
}

char	*expand_home2(t_data *data, char *input, int i, char *tmp)
{
	char	*copy;
	char	*tmp2;

	if (tmp)
	{
		copy = ft_strdup(input);
		if (copy)
			copy[i] = 0;
		tmp2 = free_join(copy, tmp);
		free(tmp);
		return (tmp2);
	}
	return (NULL);
}

int	is_tilde(char *input, int i, int quote)
{
	char	n;
	char	p;
	char	*tmp;

	p = 0;
	if (input[i] != '~')
		return (0);
	if (i > 0)
		p = input[i - 1];
	n = input[i + 1];
	if (i >= 2)
		if (!valid_identifier(input) && \
		(ft_isspace(n) || n == '/' || n == '+' || n == '-' || !n) && !quote)
			return (1);
	if ((ft_isspace(n) || n == '/' || n == '+' || n == '-' || !n) \
	&& (ft_isspace(p) || !p) && !quote)
		return (1);
	return (0);
}
