/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 15:51:53 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/20 19:13:19 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_command_path(char *path, char *command)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin("/", command);
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(path, tmp);
	free(tmp);
	return (full_path);
}

char	*find_command_path(char *all_paths, char *command)
{
	int		i;
	char	**arr;
	char	*full_path;

	i = -1;
	if (!all_paths || !command || !ft_strlen(command))
		return (NULL);
	arr = ft_split(all_paths, ':');
	if (!arr)
		return (cperror("Malloc", NULL, NULL, 1), NULL);
	while (arr && arr[++i])
	{
		full_path = create_command_path(arr[i], command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_matrix(arr);
			return (full_path);
		}
		if (full_path)
			free(full_path);
	}
	free_matrix(arr);
	return (ft_strdup(command));
}

void	ft_execve(char **env, char *cmd, char **args)
{
	char	*cmd_path;

	cmd_path = find_command_path(ft_getenv(env, "PATH"), cmd);
	execve(cmd_path, args, env);
	// cperror("execve", cmd, NULL, 1);
	perror("execve");
	exit(1);
}
