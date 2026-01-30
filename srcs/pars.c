/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:17:03 by titan             #+#    #+#             */
/*   Updated: 2026/01/30 13:30:42 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

t_mat4	mat4_align_vectors(t_vec3 start, t_vec3 dest)
{
	t_vec3	axis;
	double	cos_theta;
	t_mat4	rot;

	start = vec_normalize(start);
	dest = vec_normalize(dest);
	cos_theta = vec_dot_scal(start, dest);
	if (cos_theta > 0.9999)
	{
		mat4_initial(&rot);
		return (rot);
	}
	if (cos_theta < -0.9999)
	{
		t_vec3 up = {0, 1, 0};
		if (fabs(start.y) > 0.9)
			up = (t_vec3){0, 0, 1};
			
		axis = vec_cross(start, up);
		mat4_rotate_axis(&rot, axis, PI);
		return (rot);
	}
	axis = vec_cross(start, dest);
	double	angle = acos(cos_theta);
	mat4_rotate_axis(&rot, axis, angle);
	return (rot);
}

t_vec3	parse_vec3(char **line)
{
	t_vec3	v;

	v.x = rt_atod(line);
	skip_coma(line);
	v.y = rt_atod(line);
	skip_coma(line);
	v.z = rt_atod(line);
	return (v);
}

mlx_color	parse_color(char **line)
{
	mlx_color	color;

	color.r = rt_atod(line);
	skip_coma(line);
	color.g = rt_atod(line);
	skip_coma(line);
	color.b = rt_atod(line);
	return (color);
}
