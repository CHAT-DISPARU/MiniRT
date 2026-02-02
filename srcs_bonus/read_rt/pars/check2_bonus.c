/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 16:34:08 by titan             #+#    #+#             */
/*   Updated: 2026/02/02 12:23:18 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	check_extra_info(t_data *data, char *line, int i)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t' && *line != '\n')
			clean_exit(data, 1, "Error: Extra information on line\n", i);
		line++;
	}
}

void	check_color_val(t_data *data, double c, int i)
{
	if (c < 0.0 || c > 255.0)
		clean_exit(data, 1,
			"Error: Color values must be between 0 and 255\n", i);
}
