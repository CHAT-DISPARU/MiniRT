/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:24:04 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/19 12:03:17 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	static char	buf[12];
	char		*cursor;
	char		c;
	long		nb;

	cursor = &buf[sizeof(buf) - 1];
	*cursor-- = 0;
	nb = n;
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
	ft_putstr_fd(cursor + 1, fd);
}
