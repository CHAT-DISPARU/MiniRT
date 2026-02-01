/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:54:13 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/31 22:35:48 by titan            ###   ########.fr       */
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
	double	c;
	double	s;
	double	t;

	axis = vec_normalize(axis);
	c = cos(angle);
	s = sin(angle);
	t = 1.0 - c;
	mat4_initial(m);
	m->m[0][0] = t * axis.x * axis.x + c;
	m->m[0][1] = t * axis.x * axis.y - s * axis.z;
	m->m[0][2] = t * axis.x * axis.z + s * axis.y;
	m->m[1][0] = t * axis.x * axis.y + s * axis.z;
	m->m[1][1] = t * axis.y * axis.y + c;
	m->m[1][2] = t * axis.y * axis.z - s * axis.x;
	m->m[2][0] = t * axis.x * axis.z - s * axis.y;
	m->m[2][1] = t * axis.y * axis.z + s * axis.x;
	m->m[2][2] = t * axis.z * axis.z + c;
}
