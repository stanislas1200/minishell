/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tool2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 00:07:18 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/15 00:08:32 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_identifier(char *var)
{
	int	i;

	i = -1;
	if (ft_strlen(var) <= 1)
		return (1);
	while (var[++i] != '=')
	{
		if (i == 0 && !ft_isalpha(var[i]) && var[i] != '_')
			return (1);
		if (!ft_isalnum(var[i]) && var[i] != '_' && var[i] != '+')
			return (1);
		if (var[i] == '+' && var[i + 1] != '=')
			return (1);
	}
	return (0);
}