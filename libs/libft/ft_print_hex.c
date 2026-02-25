/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 22:14:31 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/06 16:39:25 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_xtoa(unsigned int num, int upper)
{
	static char	buf[17];
	char		*cursor;
	char		c;

	cursor = &buf[sizeof(buf) - 1];
	*cursor-- = 0;
	while (num)
	{
		c = num % 16;
		if (c >= 10)
			c += ('a' - '0') - 10 - (32 * upper);
		c += '0';
		*cursor-- = c;
		num >>= 4;
	}
	if (cursor == &buf[sizeof(buf) - 2])
		*cursor-- = '0';
	return (++cursor);
}

int	ft_print_lhex(t_values val)
{
	char	*result;

	result = ft_xtoa(val.u_value, 0);
	return (ft_putstr(result, 1));
}

int	ft_print_uhex(t_values val)
{
	char	*result;

	result = ft_xtoa(val.u_value, 1);
	return (ft_putstr(result, 1));
}
