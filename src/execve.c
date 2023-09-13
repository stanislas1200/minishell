/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 15:51:53 by dspilleb          #+#    #+#             */
/*   Updated: 2023/09/12 16:21:19 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*find_command_path(char *all_paths, char *c)
{
	int		i;
	char	**arr;
	char	*full_path;

	i = -1;
	if (!c || !ft_strlen(c))
		return (NULL);
	if (!all_paths || !ft_strncmp(c, "./", 2) || !ft_strncmp(c, "/", 1))
		return (ft_strdup(c));
	arr = ft_split(all_paths, ':');
	if (!arr)
		return (cperror("Malloc", NULL, NULL, 1), NULL);
	while (arr && arr[++i])
	{
		full_path = create_command_path(arr[i], c);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_matrix(arr);
			return (full_path);
		}
		if (full_path)
			free(full_path);
	}
	free_matrix(arr);
	return (ft_strdup(c));
}

void	ft_execve(t_data *data, char *cmd, char **args)
{
	char	*cmd_path;

	cmd_path = find_command_path(ft_getenv(data->env, "PATH"), cmd);
	signal(SIGQUIT, SIG_DFL);
	execve(cmd_path, args, data->env);
	cperror("execve", cmd, NULL, 1);
	if (cmd_path)
		free(cmd_path);
	if (cmd[0] == '.')
	{
		clean(data);
		free(args);
		exit(126);
	}
	clean(data);
	free(args);
	if (errno == EACCES || errno == ENOENT)
		exit(127);
	exit(1);
}
