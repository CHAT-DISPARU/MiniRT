/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_uinteger.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:56:57 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/08 18:59:52 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_utoa(unsigned int num)
{
	static char	buf[11];
	char		*cursor;
	char		c;

	cursor = &buf[sizeof(buf) - 1];
	*cursor-- = 0;
	while (num)
	{
		c = num % 10;
		*cursor-- = c + '0';
		num /= 10;
	}
	if (cursor == &buf[sizeof(buf) - 2])
		*cursor-- = '0';
	return (++cursor);
}

int	ft_print_uinteger(t_values val)
{
	char	*result;

	result = ft_utoa(val.u_value);
	return (ft_putstr(result, 1));
}
