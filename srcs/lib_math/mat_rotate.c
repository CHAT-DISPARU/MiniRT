/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:54:13 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/20 16:24:02 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib_mat.h>

void	mat4_rotate_y(t_mat4 *m, float angle)
{
	float	c;
	float	s;

	s = sinf(angle);
	c = cosf(angle);
	mat4_initial(m);
	m->m[0][0] = c;
	m->m[0][2] = s;
	m->m[2][0] = -s;
	m->m[2][2] = c;
}

void	mat4_rotate_x(t_mat4 *m, float angle)
{
	float	c;
	float	s;

	c = cosf(angle);
	s = sinf(angle);
	mat4_initial(m);
	m->m[1][1] = c;
	m->m[1][2] = -s;
	m->m[2][1] = s;
	m->m[2][2] = c;
}

void	mat4_rotate_z(t_mat4 *m, float angle)
{
	float	c;
	float	s;

	c = cosf(angle);
	s = sinf(angle);
	mat4_initial(m);
	m->m[0][0] = c;
	m->m[0][1] = -s;
	m->m[1][0] = s;
	m->m[1][1] = c;
}

/*
	pour rotate par rapport a un vecteur
*/

void	mat4_rotate_axis(t_mat4 *m, t_vec3 axis, double angle)
{
	double	c = cos(angle);
	double	s = sin(angle);
	double	t = 1.0 - c;
	double	x = axis.x;
	double	y = axis.y;
	double	z = axis.z;

	mat4_initial(m);
	m->m[0][0] = t * x * x + c;
	m->m[0][1] = t * x * y - s * z;
	m->m[0][2] = t * x * z + s * y;
	m->m[1][0] = t * x * y + s * z;
	m->m[1][1] = t * y * y + c;
	m->m[1][2] = t * y * z - s * x;
	m->m[2][0] = t * x * z - s * y;
	m->m[2][1] = t * y * z + s * x;
	m->m[2][2] = t * z * z + c;
}
