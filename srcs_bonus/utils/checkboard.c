/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkboard.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 12:28:16 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 13:50:29 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	get_basis(t_vec3 normal, t_vec3 *u_axis, t_vec3 *v_axis)
{
	t_vec3	up;

	up.x = 0.0;
	up.y = 1.0;
	up.z = 0.0;
	if (fabs(normal.y) > 0.999)
	{
		up.x = 1.0;
		up.y = 0.0;
		up.z = 0.0;
	}
	*u_axis = vec_normalize(vec_cross(up, normal));
	*v_axis = vec_cross(normal, *u_axis);
}

t_vec3	get_checker_color(double u, double v, t_vec3 color_a, t_vec3 color_b)
{
	double	scale;
	int		u_calc;
	int		v_calc;

	scale = 10.0;
	u_calc = (int)floor(u * scale);
	v_calc = (int)floor(v * scale);
	if ((u_calc + v_calc) % 2 == 0)
		return (color_a);
	return (color_b);
}

void	get_sphere_uv(t_vec3 normal, double *u, double *v)
{
	double	phi;
	double	theta;

	phi = atan2(normal.z, normal.x);
	theta = asin(normal.y);
	*u = 1.0 - ((phi + PI) / (2.0 * PI));
	*v = (theta + PI / 2.0) / PI;
}
