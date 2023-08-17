/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 22:04:16 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/17 04:00:30 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(char **envp, char **args)
{
	long	number;
	int		exit_code;

	errno = 0;
	exit_code = 0;
	printf(R "exit\n");
	if (matrix_len(args) > 1)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit_code = 1;
	}
	else if (matrix_len(args) == 1)
		exit_code = check_exit(args[0]);
	free_matrix(envp);
	free_matrix(args);
	exit(exit_code);
}

int	check_exit(char *str)
{
	long	number;

	if (is_numeric(str))
	{
		printf("exit: %s: numeric argument required\n", str); // à remplacer pour écrire dans le bon fd
		return (2);
	}
	number = ft_long_atoi(str);
	if (errno != 0)
	{
		printf("exit: %s: numeric argument required\n", str); // à remplacer pour écrire dans le bon fd
		errno = 0;
		return (2);
	}
	else
	{
		if (number > 255)
			number %= 256;
		if (number < 0)
		{
			number %= 256;
			number *= -1;
		}
		return (number);
	}
}

int	is_numeric(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!((str[i] >= '0' && str[i] <= '9') || str[i] == '-'))
			return (EXIT_FAILURE);
		if (str[i] == '-' && (i != 0 || !str[i + 1]))
			return (EXIT_FAILURE);
	}
	if (i == 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
