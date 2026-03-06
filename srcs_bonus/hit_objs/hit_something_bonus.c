/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_something_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:00:06 by titan             #+#    #+#             */
/*   Updated: 2026/03/06 15:48:12 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

bool	check_obj_hit(t_obj *obj, t_ray ray, t_hit_r *rec)
{
	static t_calc_f	functions[FLAG_MAX];
	static bool		is_init = false;

	if (!is_init)
	{
		init_t_calc_f(functions);
		is_init = true;
	}
	return (functions[obj->type](obj, ray, rec));
}

bool	hit_bvh(t_data *data, int node_idx, t_ray ray, t_hit_r *rec)
{
	t_bvh_node	*node;
	t_hit_bvh	shit_bvh;
	t_obj		*obj;

	if (node_idx == -1)
		return (false);
	node = &data->bvh_nodes[node_idx];
	if (!intersect_aabb(ray, node->box, rec->t))
		return (false);
	shit_bvh.hit_any = false;
	if (node->left == -1)
	{
		shit_bvh.i = 0;
		while (shit_bvh.i < node->obj_count)
		{
			obj = data->sorted_objs[node->start_idx + shit_bvh.i];
			if (check_obj_hit(obj, ray, rec))
				shit_bvh.hit_any = true;
			shit_bvh.i++;
		}
		return (shit_bvh.hit_any);
	}
	shit_bvh.hit_left = hit_bvh(data, node->left, ray, rec);
	shit_bvh.hit_right = hit_bvh(data, node->right, ray, rec);
	return (shit_bvh.hit_left || shit_bvh.hit_right);
}

void	check_pl(t_data *data, t_ray ray, t_hit_r *rec, bool *hit_any)
{
	int		i;

	i = 0;
	while (i < data->plane_count)
	{
		if (check_obj_hit(&data->plane_array[i], ray, rec))
			*hit_any = true;
		i++;
	}
}

bool	hit_someting(t_data *data, t_ray ray, t_hit_r *rec)
{
	bool	hit_any;
	int		i;

	i = 0;
	hit_any = false;
	check_pl(data, ray, rec, &hit_any);
	if (data->use_bvh && data->bvh_nodes)
	{
		if (hit_bvh(data, 0, ray, rec))
			hit_any = true;
	}
	else
	{
		i = 0;
		while (i < data->obj_count)
		{
			if (check_obj_hit(&data->array_obj[i], ray, rec))
				hit_any = true;
			i++;
		}
	}
	return (hit_any);
}
