/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 17:40:13 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/10 20:03:07 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_home(t_data *data)
{
	char	*ret;

	ret = ft_getenv(data->env, "HOME");
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
	char	n;
	char	p;

	p = 0;
	if (i > 0)
		p = input[i - 1];
	n = input[i + 1];
	if ((ft_isspace(n) || n == '/' || !n) && (ft_isspace(p) || !p))
		return (free_join(ft_strdup(get_home(data)), &input[i + 1]));
	else
		return (NULL);
}

//to be continued
if (input[data->i] == '~' && !quote)
{
	tmp = expand_home(data, input, data->i);
	if (tmp)
		return (tmp);
	result_add(data, &result, input, "~");
}
