/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:16:50 by titan             #+#    #+#             */
/*   Updated: 2026/02/12 14:59:34 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	empty_aabb(void)
{
	t_aabb	box;

	box.min = (t_vec3){INFINITY, INFINITY, INFINITY};
	box.max = (t_vec3){-INFINITY, -INFINITY, -INFINITY};
	return (box);
}

void	add_point_to_aabb(t_aabb *box, t_vec3 p)
{
	box->min = vec_min(box->min, p);
	box->max = vec_max(box->max, p);
}

t_aabb	aabb_transform_matrix(t_aabb local_box, t_mat4 matrix)
{
	t_aabb	world_box;
	t_vec3	corners[8];
	t_vec3	p;
	int		i;

	corners[0] = (t_vec3){local_box.min.x, local_box.min.y, local_box.min.z};
	corners[1] = (t_vec3){local_box.min.x, local_box.min.y, local_box.max.z};
	corners[2] = (t_vec3){local_box.min.x, local_box.max.y, local_box.min.z};
	corners[3] = (t_vec3){local_box.min.x, local_box.max.y, local_box.max.z};
	corners[4] = (t_vec3){local_box.max.x, local_box.min.y, local_box.min.z};
	corners[5] = (t_vec3){local_box.max.x, local_box.min.y, local_box.max.z};
	corners[6] = (t_vec3){local_box.max.x, local_box.max.y, local_box.min.z};
	corners[7] = (t_vec3){local_box.max.x, local_box.max.y, local_box.max.z};
	world_box.min = (t_vec3){INFINITY, INFINITY, INFINITY};
	world_box.max = (t_vec3){-INFINITY, -INFINITY, -INFINITY};
	i = 0;
	while (i < 8)
	{
		p = mat4_mult_vec3(&matrix, corners[i], 1.0);
		
		world_box.min = vec_min(world_box.min, p);
		world_box.max = vec_max(world_box.max, p);
		i++;
	}
	return (world_box);
}

static bool	is_on_edge(t_vec3 p, t_aabb box, double thick)
{
	int on_face;
	
	on_face = 0;
	if (fabs(p.x - box.min.x) < thick || fabs(p.x - box.max.x) < thick)
		on_face++;
	if (fabs(p.y - box.min.y) < thick || fabs(p.y - box.max.y) < thick)
		on_face++;
	if (fabs(p.z - box.min.z) < thick || fabs(p.z - box.max.z) < thick)
		on_face++;
	return (on_face >= 2); 
}

double	hit_aabb_edge(t_ray ray, t_aabb box)
{
	double	tmin, tmax, t_enter;
	t_vec3	inv_dir;
	t_vec3	p;

	inv_dir.x = 1.0 / ray.dir.x;
	inv_dir.y = 1.0 / ray.dir.y;
	inv_dir.z = 1.0 / ray.dir.z;
	double tx1 = (box.min.x - ray.origin.x) * inv_dir.x;
	double tx2 = (box.max.x - ray.origin.x) * inv_dir.x;
	tmin = fmin(tx1, tx2);
	tmax = fmax(tx1, tx2);
	double ty1 = (box.min.y - ray.origin.y) * inv_dir.y;
	double ty2 = (box.max.y - ray.origin.y) * inv_dir.y;
	tmin = fmax(tmin, fmin(ty1, ty2));
	tmax = fmin(tmax, fmax(ty1, ty2));
	double tz1 = (box.min.z - ray.origin.z) * inv_dir.z;
	double tz2 = (box.max.z - ray.origin.z) * inv_dir.z;
	tmin = fmax(tmin, fmin(tz1, tz2));
	tmax = fmin(tmax, fmax(tz1, tz2));
	if (tmax < tmin || tmax < 0)
		return (INFINITY);
	t_enter = tmin;
	if (t_enter < EPSILON)
		t_enter = tmax;
	if (t_enter < EPSILON)
		return (INFINITY);
	p = vec_add(ray.origin, vec_scale(ray.dir, t_enter));
	double thickness = 0.01 * t_enter; 
	if (is_on_edge(p, box, thickness))
		return (t_enter);
	return (INFINITY);
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
	double	tmin, tmax, t1, t2;
	t_vec3	inv_dir;

	inv_dir.x = 1.0 / ray.dir.x;
	inv_dir.y = 1.0 / ray.dir.y;
	inv_dir.z = 1.0 / ray.dir.z;
	t1 = (box.min.x - ray.origin.x) * inv_dir.x;
	t2 = (box.max.x - ray.origin.x) * inv_dir.x;
	tmin = fmin(t1, t2);
	tmax = fmax(t1, t2);
	t1 = (box.min.y - ray.origin.y) * inv_dir.y;
	t2 = (box.max.y - ray.origin.y) * inv_dir.y;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));
	t1 = (box.min.z - ray.origin.z) * inv_dir.z;
	t2 = (box.max.z - ray.origin.z) * inv_dir.z;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));
	if (tmax < 0 || tmin > tmax || tmin > t_max)
		return (false);
	return (true);
}
