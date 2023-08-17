/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:21:48 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/17 16:51:28 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(char **args)
{
	int	i;
	int	flag;
	int	count;

	count = 0;
	flag = 0;
	i = 0;
	while (args[++i])
	{
		if (is_option(args[i], 'n') == 0)
			flag = 1;
		else
		{
			if (count == 1)
				printf(" ");
			printf("%s", args[i]);
			count = 1;
		}
	}
	if (!flag)
		printf("\n");
	free_matrix(args);
	// return(0);
}

int	is_option(char *str, char option)
{
	int	i;

	i = 0;
	if (str[0] != '-')
		return (1);
	while (str[++i])
		if (str[i] != option)
			return (1);
	if (i == 1)
		return (1);
	return (0);
}