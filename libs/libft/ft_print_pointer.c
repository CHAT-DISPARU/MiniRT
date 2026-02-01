/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:18:23 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/06 16:40:42 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_ptr_len(unsigned long num)
{
	int	len;

	len = 0;
	while (num != 0)
	{
		len++;
		num = num / 16;
	}
	return (len);
}

void	ft_put_ptr(unsigned long num)
{
	if (num >= 16)
	{
		ft_put_ptr(num / 16);
		ft_put_ptr(num % 16);
	}
	else
	{
		if (num <= 9)
			ft_putchar_fd((num + '0'), 1);
		else
			ft_putchar_fd((num - 10 + 'a'), 1);
	}
}

int	ft_print_ptr(t_values val)
{
	int	print_l;

	print_l = 0;
	if (val.p_value == 0)
	{
		ft_putstr("(nil)", 1);
		return (5);
	}
	print_l += write(1, "0x", 2);
	ft_put_ptr((unsigned long)val.p_value);
	print_l += ft_ptr_len((unsigned long)val.p_value);
	return (print_l);
}
