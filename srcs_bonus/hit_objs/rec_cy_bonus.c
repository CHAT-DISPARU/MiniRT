/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rec_cy_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:44:25 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:45:37 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_rec_cy2(t_cy_utils utils, t_hit *hit, t_hit_r *rec, t_vec3 local_p)
{
	if (utils.hit_zone == 2)
		hit->local_normal = (t_vec3){1, 0, 0};
	else
		hit->local_normal = (t_vec3){-1, 0, 0};
	rec->u = (local_p.y + 1.0) * 0.5;
	rec->v = (local_p.z + 1.0) * 0.5;
}

t_hit_r	set_rec_cy(t_ray ray, t_cy_utils utils, t_obj *cy, t_hit hit)
{
	t_hit_r	rec;
	t_vec3	lp;
	double	phi;

	rec.obj_ptr = cy;
	rec.color = cy->color;
	rec.t = utils.closest_t;
	rec.p = vec_add(ray.origin, vec_scale(ray.dir, utils.closest_t));
	lp = vec_add(hit.l_ray.origin, vec_scale(hit.l_ray.dir, utils.closest_t));
	if (utils.hit_zone == 1)
	{
		hit.local_normal = vec_add(hit.l_ray.origin,
				vec_scale(hit.l_ray.dir, utils.closest_t));
		hit.local_normal.x = 0;
		phi = atan2(lp.z, lp.y);
		rec.u = (phi + PI) / (2.0 * PI);
		rec.v = (lp.x + 1.0) * 0.5;
	}
	else if (utils.hit_zone == 2 || utils.hit_zone == 3)
		set_rec_cy2(utils, &hit, &rec, lp);
	rec.normal = mat4_mult_vec3(&cy->transform, hit.local_normal, 0.0);
	rec.normal = vec_normalize(rec.normal);
	if (vec_dot_scal(ray.dir, rec.normal) > 0)
		rec.normal = vec_scale(rec.normal, -1.0);
	return (rec);
}
