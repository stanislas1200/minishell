/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:41:45 by dspilleb          #+#    #+#             */
/*   Updated: 2023/08/23 15:43:48 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_join(char *s1, char *s2)
{
	char	*temp;

	if (s1)
	{
		temp = ft_strjoin(s1, s2);
		free (s1);
		s1 = NULL;
		return (temp);
	}
	return (ft_strdup(s2));
}
