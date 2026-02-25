/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_something_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:00:06 by titan             #+#    #+#             */
/*   Updated: 2026/02/12 14:59:17 by titan            ###   ########.fr       */
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
	bool		hit_any;
	bool		hit_left;
	bool		hit_right;
	int			i;
	t_obj		*obj;

	if (node_idx == -1)
		return (false);
	node = &data->bvh_nodes[node_idx];
	if (!intersect_aabb(ray, node->box, rec->t))
		return (false);
	hit_any = false;
	if (node->left == -1)
	{
		i = 0;
		while (i < node->obj_count)
		{
			obj = data->sorted_objs[node->start_idx + i];
			if (check_obj_hit(obj, ray, rec))
				hit_any = true;
			i++;
		}
		return (hit_any);
	}
	hit_left = hit_bvh(data, node->left, ray, rec);
	hit_right = hit_bvh(data, node->right, ray, rec);
	return (hit_left || hit_right);
}

bool	hit_someting(t_data *data, t_ray ray, t_hit_r *rec)
{
	bool	hit_any = false;
	int		i;

	i = 0;
	while (i < data->plane_count)
	{
		if (check_obj_hit(&data->plane_array[i], ray, rec))
			hit_any = true;
		i++;
	}
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
