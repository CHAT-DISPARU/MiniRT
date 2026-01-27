/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:39:14 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/21 10:52:02 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	find_format(const char s)
{
	static const int	ascii_flags[255] = {
	['c'] = FLAG_CHAR,
	['s'] = FLAG_STRING,
	['p'] = FLAG_POINTER,
	['d'] = FLAG_DECIMAL,
	['i'] = FLAG_INTEGER,
	['u'] = FLAG_UNSIGNED,
	['x'] = FLAG_LOWHEX,
	['X'] = FLAG_UPPHEX,
	};

	return (ascii_flags[(unsigned int)s] - 1);
}

static void	init_readers(t_reader *readers)
{
	readers[FLAG_DECIMAL - 1] = read_integer;
	readers[FLAG_INTEGER - 1] = read_integer;
	readers[FLAG_CHAR - 1] = read_char;
	readers[FLAG_STRING - 1] = read_str;
	readers[FLAG_POINTER - 1] = read_pointer;
	readers[FLAG_UNSIGNED - 1] = read_uinteger;
	readers[FLAG_LOWHEX - 1] = read_uinteger;
	readers[FLAG_UPPHEX - 1] = read_uinteger;
}

static void	init_fmtfn(t_fmtfn *functions)
{
	functions[FLAG_DECIMAL - 1] = ft_print_integer;
	functions[FLAG_INTEGER - 1] = ft_print_integer;
	functions[FLAG_CHAR - 1] = ft_print_char;
	functions[FLAG_STRING - 1] = ft_print_str;
	functions[FLAG_POINTER - 1] = ft_print_ptr;
	functions[FLAG_UNSIGNED - 1] = ft_print_uinteger;
	functions[FLAG_LOWHEX - 1] = ft_print_lhex;
	functions[FLAG_UPPHEX - 1] = ft_print_uhex;
}

static void	ft_call_functions(const char s, int *len, va_list *args)
{
	int				i;
	t_values		all_types;
	static t_fmtfn	functions[FLAG_MAX];
	static t_reader	readers[FLAG_MAX];

	i = 0;
	init_readers(readers);
	init_fmtfn(functions);
	i = find_format(s);
	readers[i](args, &all_types);
	*len += functions[i](all_types);
}

int	ft_printf(const char *s, ...)
{
	int			len;
	va_list		args;

	len = 0;
	va_start(args, s);
	if (!s)
		return (-1);
	while (*s)
	{
		if (*s == '%' && *(s + 1) == '%')
			len += ft_print_percent();
		if (*s == '%' && *(s + 1) && find_format(*(s + 1)) < 0)
			s++;
		else if (*s == '%' && *(s + 1))
		{
			s++;
			ft_call_functions(*s, &len, &args);
		}
		else
			len += write (1, &(*s), 1);
		s++;
	}
	va_end(args);
	return (len);
}
