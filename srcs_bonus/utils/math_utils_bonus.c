/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:12:29 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 09:45:11 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

/*
	protection gimbal lock
*/
t_vec3	get_right_vector(t_vec3 dir)
{
	t_vec3	up_guide;
	t_vec3	right;

	up_guide = (t_vec3){0, 1, 0};
	if (fabs(vec_dot_scal(dir, up_guide)) > 0.99)
		up_guide = (t_vec3){1, 0, 0};
	right = vec_normalize(vec_cross(dir, up_guide));
	return (right);
}

void	calcul_ambient(t_data *data)
{
	double	ratio;

	ratio = data->a_ratio;
	data->a_final.x = (data->a_color.r / 255.0) * ratio;
	data->a_final.y = (data->a_color.g / 255.0) * ratio;
	data->a_final.z = (data->a_color.b / 255.0) * ratio;
}

double	rand_double(void)
{
	return ((double)rand() / (double)RAND_MAX);
}

/*
	​Rx ​Ux Fx ​Tx​​
	Ry Uy Fy Ty​​
	Rz Uz ​Fz Tz​​
	0  0  0​  1​​
*/
void	fill_look_m(t_mat4 *m, t_vec3 r, t_vec3 u, t_vec3 f)
{
	m->m[0][0] = r.x;
	m->m[0][1] = u.x;
	m->m[0][2] = f.x;
	m->m[1][0] = r.y;
	m->m[1][1] = u.y;
	m->m[1][2] = f.y;
	m->m[2][0] = r.z;
	m->m[2][1] = u.z;
	m->m[2][2] = f.z;
	m->m[3][0] = 0;
	m->m[3][1] = 0;
	m->m[3][2] = 0;
	m->m[3][3] = 1;
}

t_mat4	look_at(t_vec3 o, t_vec3 dir, t_vec3 up_guide)
{
	t_vec3	f;
	t_vec3	r;
	t_vec3	u;
	t_mat4	m;

	f = vec_normalize(vec_scale(dir, -1));
	if (fabs(vec_dot_scal(f, up_guide)) > 0.99)
		up_guide = vec_add(up_guide, (t_vec3){0.1, 0, 0});
	r = vec_normalize(vec_cross(up_guide, f));
	u = vec_cross(f, r);
	mat4_initial(&m);
	m.m[0][3] = o.x;
	m.m[1][3] = o.y;
	m.m[2][3] = o.z;
	fill_look_m(&m, r, u, f);
	return (m);
}
