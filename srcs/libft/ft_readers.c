/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 10:28:20 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/17 11:08:26 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	read_char(va_list *args, t_values *val)
{
	val->c_value = (char)va_arg(*args, int);
}

void	read_str(va_list *args, t_values *val)
{
	val->s_value = va_arg(*args, char *);
}

void	read_pointer(va_list *args, t_values *val)
{
	val->p_value = va_arg(*args, void *);
}

void	read_integer(va_list *args, t_values *val)
{
	val->i_value = va_arg(*args, int);
}

void	read_uinteger(va_list *args, t_values *val)
{
	val->u_value = va_arg(*args, unsigned int);
}
