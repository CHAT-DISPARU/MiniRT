/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:54:06 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/06 16:39:39 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_str(t_values val)
{
	int	lenght;

	if (val.s_value == NULL)
	{
		ft_putstr("(null)", 1);
		return (6);
	}
	lenght = ft_strlen(val.s_value);
	ft_putstr(val.s_value, 1);
	return (lenght);
}
