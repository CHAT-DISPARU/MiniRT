/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_c_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:59 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 13:11:32 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_viewport(t_data *data, t_vec3 origin, t_vec3 dir, double fov)
{
	data->cam.origin = origin;
	data->cam.dir = vec_normalize(dir);
	data->cam.fov = fov;
	data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
	data->view_port.focal_length = 1.0;
	data->view_port.viewport_height = 2.0
		* tan(data->view_port.fov_radians / 2.0) * data->view_port.focal_length;
	data->view_port.aspect_ratio = (double)data->width
		/ (double)data->height;
	data->view_port.viewport_width = data->view_port.aspect_ratio
		* data->view_port.viewport_height;
}

void	set_c(t_data *data, char *line, int i)
{
	t_vec3	origin;
	t_vec3	dir;
	double	fov;
	char	*line2;

	line2 = line + 1;
	if (data->camera_is_set == true)
		clean_exit(data, 1, "Error: Multiple cameras defined\n", i);
	check_missing_info(data, line2, i);
	origin = parse_vec3(&line2, data, i);
	check_missing_info(data, line2, i);
	dir = parse_vec3(&line2, data, i);
	check_norm_vec(data, dir, i);
	check_missing_info(data, line2, i);
	fov = rt_atod(&line2);
	check_fov(data, fov, i);
	check_extra_info(data, line2, i);
	set_viewport(data, origin, dir, fov);
	data->camera_is_set = true;
}
