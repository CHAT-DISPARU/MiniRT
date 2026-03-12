/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:16:50 by titan             #+#    #+#             */
/*   Updated: 2026/03/11 10:43:47 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

double	hit_aabb_edge(t_ray ray, t_aabb box)
{
	t_vec3		inv_dir;
	t_vec3		p;
	t_aabb_edge	utils;

	inv_dir.x = 1.0 / ray.dir.x;
	inv_dir.y = 1.0 / ray.dir.y;
	inv_dir.z = 1.0 / ray.dir.z;
	set_tcoords(&utils, ray, box, inv_dir);
	if (utils.tmax < utils.tmin || utils.tmax < 0)
		return (DBL_MAX);
	utils.t_enter = utils.tmin;
	if (utils.t_enter < EPSILON)
		utils.t_enter = utils.tmax;
	if (utils.t_enter < EPSILON)
		return (DBL_MAX);
	p = vec_add(ray.origin, vec_scale(ray.dir, utils.t_enter));
	utils.thickness = 0.006 * utils.t_enter;
	if (is_on_edge(p, box, utils.thickness))
		return (utils.t_enter);
	return (DBL_MAX);
}

t_aabb	aabb_union(t_aabb a, t_aabb b)
{
	t_aabb	res;

	res.min.x = fmin(a.min.x, b.min.x);
	res.min.y = fmin(a.min.y, b.min.y);
	res.min.z = fmin(a.min.z, b.min.z);
	res.max.x = fmax(a.max.x, b.max.x);
	res.max.y = fmax(a.max.y, b.max.y);
	res.max.z = fmax(a.max.z, b.max.z);
	return (res);
}

t_aabb	compute_bounds(t_obj **objs, int count)
{
	t_aabb	global_box;
	t_aabb	current_box;
	int		i;

	if (count <= 0)
	{
		ft_bzero(&global_box, sizeof(t_aabb));
		return (global_box);
	}
	global_box = get_aabb_by_type(objs[0]);
	i = 1;
	while (i < count)
	{
		current_box = get_aabb_by_type(objs[i]);
		global_box = aabb_union(global_box, current_box);
		i++;
	}
	return (global_box);
}

bool	intersect_aabb(t_ray ray, t_aabb box, double t_max)
{
	double	tmin;
	double	tmax;
	t_vec2	t;
	t_vec3	inv_dir;

	inv_dir.x = 1.0 / ray.dir.x;
	inv_dir.y = 1.0 / ray.dir.y;
	inv_dir.z = 1.0 / ray.dir.z;
	t.x = (box.min.x - ray.origin.x) * inv_dir.x;
	t.y = (box.max.x - ray.origin.x) * inv_dir.x;
	tmin = fmin(t.x, t.y);
	tmax = fmax(t.x, t.y);
	t.x = (box.min.y - ray.origin.y) * inv_dir.y;
	t.y = (box.max.y - ray.origin.y) * inv_dir.y;
	tmin = fmax(tmin, fmin(t.x, t.y));
	tmax = fmin(tmax, fmax(t.x, t.y));
	t.x = (box.min.z - ray.origin.z) * inv_dir.z;
	t.y = (box.max.z - ray.origin.z) * inv_dir.z;
	tmin = fmax(tmin, fmin(t.x, t.y));
	tmax = fmin(tmax, fmax(t.x, t.y));
	if (tmax < 0 || tmin > tmax || tmin > t_max)
		return (false);
	return (true);
}

double	get_ab_s(t_aabb box)
{
	t_vec3	size;

	size = vec_sub(box.max, box.min);
	return (2.0 * ((size.x * size.y) + (size.y * size.z) + (size.z * size.x)));
}
