/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:52:02 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/06 16:38:55 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

typedef enum e_flag
{
	FLAG_CHAR = 1,
	FLAG_STRING,
	FLAG_POINTER,
	FLAG_DECIMAL,
	FLAG_INTEGER,
	FLAG_UNSIGNED,
	FLAG_LOWHEX,
	FLAG_UPPHEX,
	FLAG_MAX,
}		t_flag;

typedef union u_values
{
	int				i_value;
	char			*s_value;
	char			c_value;
	unsigned int	u_value;
	void			*p_value;
}		t_values;

typedef int			(*t_fmtfn)(t_values);
typedef void		(*t_reader)(va_list *args, t_values *val);
int			ft_printf(const char *s, ...)
			__attribute__ ((format (printf, 1, 2)));
int			ft_putstr(char *s, int fd);
int			ft_print_char(t_values val);
int			ft_print_integer(t_values val);
int			ft_print_lhex(t_values val);
int			ft_print_percent(void);
int			ft_print_ptr(t_values val);
int			ft_print_str(t_values val);
int			ft_print_uhex(t_values val);
int			ft_print_uinteger(t_values val);
void		read_char(va_list *args, t_values *val);
void		read_str(va_list *args, t_values *val);
void		ft_putchar_fd(char c, int fd);
void		read_pointer(va_list *args, t_values *val);
void		read_integer(va_list *args, t_values *val);
void		read_uinteger(va_list *args, t_values *val);
size_t		ft_strlen(const char *s);

#endif
