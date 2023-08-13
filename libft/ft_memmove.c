/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 12:29:35 by sgodin            #+#    #+#             */
/*   Updated: 2022/10/27 14:59:31 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char	*p;
	unsigned char	*q;

	if (!dst && !src)
		return (dst);
	p = (unsigned char *)dst;
	q = (unsigned char *)src;
	if (p < q)
		while (n--)
			*(p++) = *(q++);
	else
		while (n--)
			p[n] = q[n];
	return (dst);
}
