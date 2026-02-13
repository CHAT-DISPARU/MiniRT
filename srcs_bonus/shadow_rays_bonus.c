/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_rays_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 14:41:51 by titan             #+#    #+#             */
/*   Updated: 2026/02/11 15:29:45 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

bool	call_func(t_obj *objs, t_ray shadow_ray, double light_dist)
{
	static t_calc_f	functions[FLAG_MAX];
	static bool		is_init = false;
	t_hit_r			shadow_rec;

	if (!is_init)
	{
		init_t_calc_f(functions);
		is_init = true;
	}
	shadow_rec.t = light_dist;
	if (functions[objs->type](objs, shadow_ray, &shadow_rec))
	{
		if (shadow_rec.t < light_dist && shadow_rec.t > EPSILON)
			return (true);
	}
	return (false);
}

bool	is_in_shadow(t_data *data, t_hit_r *rec, t_light *light)
{
	t_ray	shadow_ray;
	t_vec3	light_dir;
	double	light_dist;
	t_hit_r	shadow_rec;

	light_dir = vec_sub(light->origin, rec->p);
	light_dist = sqrt(vec_dot_scal(light_dir, light_dir));
	shadow_ray.dir = vec_normalize(light_dir);
	shadow_ray.origin = vec_add(rec->p, vec_scale(rec->normal, EPSILON));
	shadow_rec.t = light_dist;
	if (data->use_bvh && data->bvh_nodes)
	{
		if (hit_bvh(data, 0, shadow_ray, &shadow_rec))
			return (true);
	}
	else
	{
		int i = 0;
		while (i < data->obj_count)
		{
			if (call_func(&data->array_obj[i], shadow_ray, light_dist)) 
				return (true);
			i++;
		}
	}
	int j = 0;
	while (j < data->plane_count)
	{
		if (call_func(&data->plane_array[j], shadow_ray, light_dist))
			return (true);
		j++;
	}
	return (false);
}
