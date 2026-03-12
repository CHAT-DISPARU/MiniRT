/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils_part2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:40:53 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:43:47 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	empty_aabb(void)
{
	t_aabb	box;

	box.min = (t_vec3){DBL_MAX, DBL_MAX, DBL_MAX};
	box.max = (t_vec3){-DBL_MAX, -DBL_MAX, -DBL_MAX};
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
	world_box.min = (t_vec3){DBL_MAX, DBL_MAX, DBL_MAX};
	world_box.max = (t_vec3){-DBL_MAX, -DBL_MAX, -DBL_MAX};
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

bool	is_on_edge(t_vec3 p, t_aabb box, double thick)
{
	int	on_face;

	on_face = 0;
	if (fabs(p.x - box.min.x) < thick || fabs(p.x - box.max.x) < thick)
		on_face++;
	if (fabs(p.y - box.min.y) < thick || fabs(p.y - box.max.y) < thick)
		on_face++;
	if (fabs(p.z - box.min.z) < thick || fabs(p.z - box.max.z) < thick)
		on_face++;
	return (on_face >= 2);
}

void	set_tcoords(t_aabb_edge *utils, t_ray ray, t_aabb box, t_vec3 inv_dir)
{
	utils->tx1 = (box.min.x - ray.origin.x) * inv_dir.x;
	utils->tx2 = (box.max.x - ray.origin.x) * inv_dir.x;
	utils->tmin = fmin(utils->tx1, utils->tx2);
	utils->tmax = fmax(utils->tx1, utils->tx2);
	utils->ty1 = (box.min.y - ray.origin.y) * inv_dir.y;
	utils->ty2 = (box.max.y - ray.origin.y) * inv_dir.y;
	utils->tmin = fmax(utils->tmin, fmin(utils->ty1, utils->ty2));
	utils->tmax = fmin(utils->tmax, fmax(utils->ty1, utils->ty2));
	utils->tz1 = (box.min.z - ray.origin.z) * inv_dir.z;
	utils->tz2 = (box.max.z - ray.origin.z) * inv_dir.z;
	utils->tmin = fmax(utils->tmin, fmin(utils->tz1, utils->tz2));
	utils->tmax = fmin(utils->tmax, fmax(utils->tz1, utils->tz2));
}
