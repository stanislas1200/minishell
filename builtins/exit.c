/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 22:04:16 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/12 16:32:44 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		check_exit(char *str);
int		is_numeric(char *str);

int	ft_exit(char **envp, char **args, t_data *data)
{
	long	number;
	int		exit_code;

	errno = 0;
	exit_code = 0;
	if (!data->in_pipe)
		printf(R "exit\n" C);
	if (matrix_len(args) > 2)
	{
		cperror("exit", NULL, "too many arguments", 0);
		return (1);
	}
	else if (matrix_len(args) == 2)
		exit_code = check_exit(args[1]);
	clean(data);
	free(args);
	exit(exit_code);
	return (0);
}

int	check_exit(char *str)
{
	long	number;

	if (is_numeric(str))
		return (cperror("exit", str, "numeric argument required", 0), 255);
	number = ft_long_atoi(str);
	if (errno != 0)
	{
		errno = 0;
		return (cperror("exit", str, "numeric argument required", 0), 255);
	}
	else
	{
		if (number > 255)
			number = number % 256;
		else if (number < 0)
			number = 255 - (((number * -1) - 1) % 256);
		return (number);
	}
}

int	is_numeric(char *str)
{
	int	flag;
	int	i;

	i = -1;
	flag = 0;
	while (str[++i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			flag = 1;
		if (!((str[i] >= '0' && str[i] <= '9') || \
		(str[i] == '-' || str[i] == '+' || ft_isspace(str[i]))))
			return (EXIT_FAILURE);
		if ((str[i] == '-' || str[i] == '+') && (i != 0 || !str[i + 1]))
			return (EXIT_FAILURE);
	}
	if (i == 0 || !flag)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
