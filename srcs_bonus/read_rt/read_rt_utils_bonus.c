/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_rt_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:38:20 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:39:46 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	maj_setters(t_data *data, char **ptr, int i)
{
	if (**ptr == 'A' && is_space((*ptr)[1]))
		set_a(data, *ptr, i);
	else if (**ptr == 'O' && is_space((*ptr)[1]))
		set_o(data, *ptr, i);
	else if (**ptr == 'C' && is_space((*ptr)[1]))
		set_c(data, *ptr, i);
	else if (**ptr == 'L' && is_space((*ptr)[1]))
		set_l(data, *ptr, i);
}

void	call_setters(t_data *data, char *ptr, int i)
{
	if (ft_isalphamaj(*ptr))
		maj_setters(data, &ptr, i);
	else if (!ft_strncmp("sp", ptr, 2) && is_space(ptr[2]))
		set_sp(data, ptr + 2, i);
	else if (!ft_strncmp("pl", ptr, 2) && is_space(ptr[2]))
		set_pl(data, ptr + 2, i);
	else if (!ft_strncmp("cy", ptr, 2) && is_space(ptr[2]))
		set_cy(data, ptr + 2, i);
	else if (!ft_strncmp("sq", ptr, 2) && is_space(ptr[2]))
		set_sq(data, ptr + 2, i);
	else if (!ft_strncmp("hy", ptr, 2) && is_space(ptr[2]))
		set_hy(data, ptr + 2, i);
	else if (!ft_strncmp("tr", ptr, 2) && is_space(ptr[2]))
		set_tr(data, ptr + 2, i);
	else if (!ft_strncmp("co", ptr, 2) && is_space(ptr[2]))
		set_co(data, ptr + 2, i);
	else if (*ptr == '#' || !ft_strncmp("//", ptr, 2))
		return ;
	else
		clean_exit(data, EXIT_FAILURE, "Wrong identifier\n", i);
}
