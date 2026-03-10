/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_rays_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 14:41:51 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 13:54:39 by gajanvie         ###   ########.fr       */
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
		{
			if (objs->opacity == 1.0 || objs->opacity >= 0.999999999999)
				return (true);
		}
	}
	return (false);
}

bool	hit_bvh_shadow(t_data *data, int node_idx, t_ray ray,
			double light_dist)
{
	t_bvh_node	*node;
	int			i;
	t_obj		*obj;

	if (node_idx == -1)
		return (false);
	node = &data->bvh_nodes[node_idx];
	if (!intersect_aabb(ray, node->box, light_dist))
		return (false);
	if (node->left == -1)
	{
		i = 0;
		while (i < node->obj_count)
		{
			obj = data->sorted_objs[node->start_idx + i];
			if (call_func(obj, ray, light_dist))
				return (true);
			i++;
		}
		return (false);
	}
	if (hit_bvh_shadow(data, node->left, ray, light_dist))
		return (true);
	return (hit_bvh_shadow(data, node->right, ray, light_dist));
}

bool	hit_no_bvh(t_ray shadow_ray,
	double light_dist, t_data *data, t_vec2int *idx)
{
	idx->x = 0;
	while (idx->x < data->obj_count)
	{
		if (call_func(&data->array_obj[idx->x], shadow_ray, light_dist))
			return (true);
		idx->x++;
	}
	return (false);
}

bool	is_in_shadow(t_data *data, t_hit_r *rec, t_light *light)
{
	t_ray		shadow_ray;
	t_vec3		light_dir;
	double		light_dist;
	t_vec2int	idx;

	light_dir = vec_sub(light->origin, rec->p);
	light_dist = sqrt(vec_dot_scal(light_dir, light_dir));
	shadow_ray.dir = vec_normalize(light_dir);
	shadow_ray.origin = vec_add(rec->p, vec_scale(rec->normal, EPSILON));
	if (data->use_bvh && data->bvh_nodes)
		return (hit_bvh_shadow(data, 0, shadow_ray, light_dist));
	else
	{
		if (hit_no_bvh(shadow_ray, light_dist, data, &idx))
			return (true);
	}
	idx.y = 0;
	while (idx.y < data->plane_count)
	{
		if (call_func(&data->plane_array[idx.y], shadow_ray, light_dist))
			return (true);
		idx.y++;
	}
	return (false);
}
