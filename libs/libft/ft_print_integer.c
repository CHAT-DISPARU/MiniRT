/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_integer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:14:52 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/06 16:39:31 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_complex_itoa(int num)
{
	static char	buf[12];
	char		*cursor;
	char		c;
	long int	nb;

	cursor = &buf[sizeof(buf) - 1];
	*cursor-- = 0;
	nb = num;
	while (nb)
	{
		c = nb % 10;
		if (nb < 0)
			c = -nb % 10;
		*cursor-- = c + '0';
		if (nb > -10 && nb < 0)
			*cursor-- = '-';
		nb /= 10;
	}
	if (cursor == &buf[sizeof(buf) - 2])
		*cursor-- = '0';
	return (++cursor);
}

int	ft_print_integer(t_values val)
{
	char	*result;

	result = ft_complex_itoa(val.i_value);
	return (ft_putstr(result, 1));
}
