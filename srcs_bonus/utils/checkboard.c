/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkboard.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 12:28:16 by titan             #+#    #+#             */
/*   Updated: 2026/02/13 13:13:43 by titan            ###   ########.fr       */
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
	double	scale = 10.0;
	int		u_calc;
	int		v_calc;

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

void	get_pl_uv(t_vec3 p, t_vec3 normal, double *u, double *v)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	double	u_val;
	double	v_val;
	double	scale;

	scale = 0.1;
	get_basis(normal, &u_axis, &v_axis);
	u_val = vec_dot_scal(p, u_axis) * scale;
	v_val = vec_dot_scal(p, v_axis) * scale;
	*u = u_val - floor(u_val);
	*v = v_val - floor(v_val);
}

void	get_sq_uv(t_vec3 p, t_vec3 center, t_vec3 normal, double side_size, double *u, double *v)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	p_local;

	get_basis(normal, &u_axis, &v_axis);
	p_local = vec_sub(p, center);
	*u = (vec_dot_scal(p_local, u_axis) / side_size) + 0.5;
	*v = (vec_dot_scal(p_local, v_axis) / side_size) + 0.5;
	if (*u < 0.0)
		*u = 0.0;
	if (*u > 1.0)
		*u = 1.0;
	if (*v < 0.0)
		*v = 0.0;
	if (*v > 1.0)
		*v = 1.0;
}

void	get_cycohy_uv(t_vec3 p, t_vec3 center, t_vec3 axis, double height, double *u, double *v)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	p_local;
	double	proj_u;
	double	proj_v;

	get_basis(axis, &u_axis, &v_axis);
	p_local = vec_sub(p, center);
	proj_u = vec_dot_scal(p_local, u_axis);
	proj_v = vec_dot_scal(p_local, v_axis);
	*u = 0.5 + (atan2(proj_v, proj_u) / (2.0 * PI));
	*v = (vec_dot_scal(p_local, axis) / height) + 0.5;
	if (*v < 0.0)
		*v = 0.0;
	if (*v > 1.0)
		*v = 1.0;
}
