/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_l_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:47 by titan             #+#    #+#             */
/*   Updated: 2026/02/02 16:20:01 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_l(t_data *data, char *line, int i)
{
	t_light	*new_light;
	t_light	light;

	line++;
	check_missing_info(data, line, i);
	light.origin = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	light.ratio = rt_atod(&line);
	check_ratio(data, light.ratio, i);
	check_missing_info(data, line, i);
	light.color = parse_color(&line, data, i);
	check_extra_info(data, line, i);
	new_light = malloc(sizeof(t_light));
	if (!new_light)
		clean_exit(data, 1, "malloc fail\n", 0);
	*new_light = light;
	new_light->next = NULL;
	ft_lightadd_back(&data->light, new_light);
}
