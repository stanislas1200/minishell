/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 17:40:13 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/12 15:41:26 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		expand(t_data *data, char **result, char *input, int var_start);
int		result_add(t_data *data, char **result, char *input, char *tmp);

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

char	*expand_home(t_data *data, char *input, int i)
{
	char	*tmp;
	char	n;
	char	p;

	p = 0;
	if (i > 0)
		p = input[i - 1];
	n = input[i + 1];
	if ((ft_isspace(n) || n == '/' || !n) && (ft_isspace(p) || !p))
	{
		tmp = get_home(data->env);
		if (tmp)
			tmp = ft_strdup(tmp);
		if (tmp)
			return (free_join(tmp, &input[i + 1]));
	}
	return (NULL);
}

// char	*expand_variables2(char *input, t_data *data, char *result, int quote)
// {
// 	char	*tmp;

// 	return (NULL);
// }
