/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_pl_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:59:19 by titan             #+#    #+#             */
/*   Updated: 2026/02/03 22:48:05 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

/*
	rayon = t*D + O

	plan : y = 0


	t*D + O = 0

	t*D = -O
	t = -O /D
*/

bool	hit_plane(t_obj *pl, t_ray ray, t_hit_r *rec)
{
	t_ray	l_ray;
	double	t;
	double	denom;
	t_vec3	local_normal;

	l_ray.origin = mat4_mult_vec3(&pl->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&pl->inverse_transform, ray.dir, 0.0);
	denom = l_ray.dir.y;
	if (fabs(denom) < EPSILON)
		return (false);
	t = -l_ray.origin.y / denom;
	if (t < EPSILON)
		return (false);
	rec->t = t;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, t));
	local_normal = (t_vec3){0, 1, 0};
	rec->normal = mat4_mult_vec3(&pl->transform, local_normal, 0.0);
	rec->normal = vec_normalize(rec->normal);
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	return (true);
}
