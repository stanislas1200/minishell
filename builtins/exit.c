/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 22:04:16 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/02 11:41:16 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		check_exit(char *str);
int		is_numeric(char *str);

void	ft_exit(char **envp, char **args, t_data *data)
{
	long	number;
	int		exit_code;

	errno = 0;
	exit_code = 0;
	// printf(R "exit\n");
	if (matrix_len(args) > 2)
	{
		cperror("exit", NULL, "too many arguments", 0);
		exit(1);
	}
	else if (matrix_len(args) == 2)
		exit_code = check_exit(args[1]);
	ast_destroy(data->ast_root);
	free_matrix(data->env);
	exit(exit_code);
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
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!((str[i] >= '0' && str[i] <= '9') || \
		(str[i] == '-' || str[i] == '+')))
			return (EXIT_FAILURE);
		if ((str[i] == '-' || str[i] == '+') && (i != 0 || !str[i + 1]))
			return (EXIT_FAILURE);
	}
	if (i == 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
