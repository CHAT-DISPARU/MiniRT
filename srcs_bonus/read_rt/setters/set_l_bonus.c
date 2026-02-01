/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_l_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:47 by titan             #+#    #+#             */
/*   Updated: 2026/02/01 13:55:55 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_l(t_data *data, char *line, int i)
{
	char	*line2;

	if (data->light_is_set)
		clean_exit(data, 1, "Error: Multi Light\n", i);
	line2 = line + 1;
	check_missing_info(data, line2, i);
	data->light.origin = parse_vec3(&line2, data, i);
	check_missing_info(data, line2, i);
	data->light.ratio = rt_atod(&line2);
	check_ratio(data, data->light.ratio, i);
	check_missing_info(data, line2, i);
	data->light.color = parse_color(&line2, data, i);
	check_extra_info(data, line2, i);
	data->light_is_set = true;
}
