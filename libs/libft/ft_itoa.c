/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:52:19 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/27 17:20:32 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int num)
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
