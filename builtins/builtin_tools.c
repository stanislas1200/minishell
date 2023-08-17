/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 03:36:58 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/17 05:59:52 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cperror(char *error, char *arg, int p_err)
{
	ft_putstr_fd(R, 2);
	if (error)
	{
		ft_putstr_fd(error, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (p_err == 1)
		perror("");
}

void	free_matrix(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
}

int	matrix_len(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

long	ft_long_atoi(const char *nptr)
{
	int		i;
	int		sign;
	long	number;

	number = 0;
	sign = 1;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if ((number > LONG_MAX / 10 || \
		(number == LONG_MAX / 10 && nptr[i] - '0' > \
		LONG_MAX % 10) && sign == 1) \
		|| (sign == -1 && (number == LONG_MAX / 10 && \
		nptr[i] - '0' > (LONG_MAX % 10) + 1)))
			return (errno = 22, -1);
		number = number * 10;
		number = number + nptr[i] - 48;
		i++;
	}
	return (number * sign);
}
