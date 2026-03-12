/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_part2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:03:53 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 11:04:26 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

/*
	rebond
	R=I−2(I⋅N)N
*/

t_ray	calc_ray(t_render_v rv, t_data *data)
{
	t_vec3	local_dir;
	t_ray	ray;

	local_dir.x = (rv.u * data->view_port.viewport_width)
		- (data->view_port.viewport_width / 2);
	local_dir.y = (data->view_port.viewport_height / 2)
		- (rv.v * data->view_port.viewport_height);
	local_dir.z = -data->view_port.focal_length;
	ray.origin = rv.cam_origin;
	ray.dir = mat4_mult_vec3(&rv.cam_m, local_dir, 0.0);
	ray.dir = vec_normalize(ray.dir);
	return (ray);
}

t_vec3	vec_random_in_unit_sphere(void)
{
	t_vec3	p;

	while (1)
	{
		p.x = rand_double() * 2.0 - 1.0;
		p.y = rand_double() * 2.0 - 1.0;
		p.z = rand_double() * 2.0 - 1.0;
		if (vec_dot_scal(p, p) < 1.0)
			return (p);
	}
}

t_vec3	random_hemisphere_dir(t_vec3 normal)
{
	t_vec3	dir;

	dir = vec_random_in_unit_sphere();
	if (vec_dot_scal(dir, normal) < 0.0)
		dir = vec_scale(dir, -1.0);
	return (vec_normalize(dir));
}

void	set_hit_col(t_hit_r *rec, t_data *data)
{
	t_vec3	base_color;
	t_vec3	col_a;
	t_vec3	col_b;

	col_a.x = rec->obj_ptr->color.r / 255.0;
	col_a.y = rec->obj_ptr->color.g / 255.0;
	col_a.z = rec->obj_ptr->color.b / 255.0;
	if (rec->obj_ptr->has_texture && rec->obj_ptr->tex)
		rec->color = get_texture_color(rec->obj_ptr->tex, rec->u, rec->v);
	if (rec->obj_ptr->bump && rec->obj_ptr->has_bump)
		apply_bump(rec, rec->obj_ptr->bump, 10);
	if (data->has_checker)
	{
		col_b.x = data->checker_color.r / 255.0;
		col_b.y = data->checker_color.g / 255.0;
		col_b.z = data->checker_color.b / 255.0;
		base_color = get_checker_color(rec->u, rec->v, col_a, col_b);
		rec->color.r = base_color.x * 255.0;
		rec->color.g = base_color.y * 255.0;
		rec->color.b = base_color.z * 255.0;
	}
}

void	debug_bvh(t_vec3 *color_acc, t_data *data, t_hit_r *rec, t_ray ray)
{
	int			k;
	double		box_t;
	t_bvh_node	*node;
	bool		should_draw;

	if (data->debug && data->bvh_nodes)
	{
		k = 0;
		while (k < data->nodes_used)
		{
			node = &data->bvh_nodes[k];
			should_draw = (node->depth <= data->debug_depth);
			if (should_draw)
			{
				box_t = hit_aabb_edge(ray, node->box);
				if (box_t < DBL_MAX && box_t < rec->t)
				{
					*color_acc = node->debug_color;
					rec->t = box_t;
				}
			}
			k++;
		}
	}
}
