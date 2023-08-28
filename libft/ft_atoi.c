/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 09:44:43 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/28 16:30:48 by dspilleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>

int	ft_overfloaw(int neg)
{
	if (neg == 1)
		return (-1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		neg;
	long	temp;
	long	nb;

	i = 0;
	neg = 1;
	nb = 0;
	temp = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		temp = str[i++] - '0' + temp * 10;
		if (temp * neg > 2147483647 || temp * neg < -2147483648)
			return (errno = 22, ft_overfloaw(neg));
		nb = temp;
	}
	return (nb * neg);
}
