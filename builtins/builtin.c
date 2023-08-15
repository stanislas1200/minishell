/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:18:44 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/14 11:43:07 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd(char *path)
{
	int		state;
	char	cwd[1024];
	char	*absolute_path;

	if (getcwd(cwd, sizeof(cwd)))
	{
		absolute_path = ft_strjoin(cwd, path);
		if (absolute_path)
		{
			state = chdir(absolute_path);
			free(absolute_path);
		}
		else
			perror("malloc");
	}
	else
		perror("getcwd");
	if (state == -1)
	{
		state = chdir(path);
		if (state == -1)
			cperror("cd", path);
	}
}
