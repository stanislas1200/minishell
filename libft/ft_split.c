/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 13:45:20 by sgodin            #+#    #+#             */
/*   Updated: 2022/11/02 18:57:38 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_number(char const *s1, char c)
{
	int	ret;
	int	check;

	ret = 0;
	check = 0;
	if (!*s1)
		return (0);
	while (*s1)
	{
		if (*s1 == c)
			check = 0;
		else if (check == 0)
		{
			check = 1;
			ret++;
		}
		s1++;
	}
	return (ret);
}

static int	ft_char_number(char const *s2, char c, int i)
{
	int	lenght;

	lenght = 0;
	while (s2[i] != c && s2[i])
	{
		lenght++;
		i++;
	}
	return (lenght);
}

char	**ft_malloc_error(char **tab, int j)
{
	while (j > 0)
	{
		j--;
		free((void *)tab[j]);
	}
	free(tab);
	return (NULL);
}

static char	**ft_affect(char const *s, char **tab, char c, int l)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (s[i] && j < l)
	{
		k = 0;
		while (s[i] == c)
			i++;
		tab[j] = (char *)malloc(sizeof(char) * ft_char_number(s, c, i) + 1);
		if (!tab[j])
			return (ft_malloc_error(tab, j));
		while (s[i] && s[i] != c)
			tab[j][k++] = s[i++];
		tab[j++][k] = 0;
	}
	tab[j] = 0;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;

	if (!s)
		return (NULL);
	i = ft_word_number(s, c);
	tab = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tab)
		return (NULL);
	return (ft_affect(s, tab, c, i));
}
