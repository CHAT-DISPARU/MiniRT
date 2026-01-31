/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_rays.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 14:41:51 by titan             #+#    #+#             */
/*   Updated: 2026/01/31 14:51:02 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

bool	is_in_shadow(t_data *data, t_hit_r *rec, t_light *light)
{
	t_ray	shadow_ray;
	t_vec3	light_dir;
	double	light_dist;
	t_obj	*objs;
	t_hit_r	shadow_rec;
	static t_calc_f functions[FLAG_MAX];
	static bool		is_init = false;

	if (!is_init)
	{
		init_t_calc_f(functions);
		is_init = true;
	}
	light_dir = vec_sub(light->origin, rec->p);
	light_dist = sqrt(vec_dot_scal(light_dir, light_dir));
	shadow_ray.dir = vec_normalize(light_dir);
	shadow_ray.origin = vec_add(rec->p, vec_scale(rec->normal, 0.001));
	objs = data->objs;
	while (objs)
	{
		if (functions[objs->type](objs, shadow_ray, &shadow_rec))
		{
			if (shadow_rec.t > 0.001 && shadow_rec.t < light_dist)
				return (true);
		}
		objs = objs->next;
	}
	return (false);
}
