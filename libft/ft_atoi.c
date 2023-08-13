/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 09:44:43 by sgodin            #+#    #+#             */
/*   Updated: 2022/11/16 13:49:41 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		nb = str[i++] - '0' + nb * 10;
		if (nb < temp)
			return (ft_overfloaw(neg));
		temp = nb;
	}
	return (nb * neg);
}
