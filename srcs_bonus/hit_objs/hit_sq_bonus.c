/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sq_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:59:36 by titan             #+#    #+#             */
/*   Updated: 2026/02/13 13:15:00 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

bool	hit_square(t_obj *sq, t_ray ray, t_hit_r *rec)
{
	t_ray	l_ray;
	double	t;
	double	denom;
	t_vec3	p;

	l_ray.origin = mat4_mult_vec3(&sq->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&sq->inverse_transform, ray.dir, 0.0);
	denom = l_ray.dir.y;
	if (fabs(denom) < EPSILON)
		return (false);
	t = -l_ray.origin.y / denom;
	if (t < EPSILON || t > rec->t)
		return (false);
	p = vec_add(l_ray.origin, vec_scale(l_ray.dir, t));
	if (fabs(p.x) > 1.0 || fabs(p.z) > 1.0)
		return (false);
	rec->color = sq->color;
	rec->t = t;
	rec->obj_ptr = sq;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, t));
	rec->u = (p.x + 1.0) * 0.5;
	rec->v = (p.z + 1.0) * 0.5;
	rec->normal = (t_vec3){0, 1, 0};
	rec->normal = mat4_mult_vec3(&sq->transform, rec->normal, 0.0);
	rec->normal = vec_normalize(rec->normal);
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	return (true);
}
