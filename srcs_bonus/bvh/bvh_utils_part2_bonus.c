/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils_part2_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: CHAT-DISPARU <CHAT-DISPARU@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:40:53 by gajanvie          #+#    #+#             */
/*   Updated: 2026/04/19 11:15:20 by CHAT-DISPAR      ###   ########.fr       */
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

void	put_pixel_safe(t_data *data, int x, int y, t_vec3 color)
{
	int	idx;

	if (x >= 0 && x < data->width && y >= 0 && y < data->height)
	{
		idx = y * data->width + x;
		data->pixels[idx].rgba = ((int)(color.x * 255.99) << 24) |
								 ((int)(color.y * 255.99) << 16) |
								 ((int)(color.z * 255.99) << 8)  | 0xFF;
	}
}

void	draw_line(t_data *data, t_point2d p0, t_point2d p1, t_vec3 color)
{
	int	dx = abs(p1.x - p0.x);
	int	dy = abs(p1.y - p0.y);
	int	sx = p0.x < p1.x ? 1 : -1;
	int	sy = p0.y < p1.y ? 1 : -1;
	int	err = (dx > dy ? dx : -dy) / 2;
	int	e2;

	while (1)
	{
		put_pixel_safe(data, p0.x, p0.y, color);
		if (p0.x == p1.x && p0.y == p1.y)
			break;
		e2 = err;
		if (e2 > -dx) { err -= dy; p0.x += sx; }
		if (e2 < dy)  { err += dx; p0.y += sy; }
	}
}

t_point2d	project_point(t_data *data, t_vec3 pt)
{
	t_point2d	p2d;
	t_vec3		cam_to_pt;
	t_vec3		right;
	t_vec3		true_up;
	double		z;
	double		x;
	double		y;
	double		fov_scale;

	cam_to_pt = vec_sub(pt, data->cam.origin);
	z = vec_dot_scal(cam_to_pt, data->cam.dir);
	if (z <= 0.001) 
	{
		p2d.z_valid = 0;
		return (p2d);
	}
	right = get_right_vector(data->cam.dir);
	true_up = vec_normalize(vec_cross(right, data->cam.dir));
	x = vec_dot_scal(cam_to_pt, right);
	y = vec_dot_scal(cam_to_pt, true_up);
	fov_scale = (data->width / 2.0) / tan(data->cam.fov * 0.5 * PI / 180.0);
	p2d.x = (int)((x / z) * fov_scale + (data->width / 2.0));
	p2d.y = (int)((data->height / 2.0) - (y / z) * fov_scale); 
	p2d.z_valid = 1;
	return (p2d);
}

void	draw_aabb_lines(t_data *data, t_aabb box, t_vec3 color)
{
	t_vec3		c[8];
	t_point2d	p[8];
	int			i;

	// Definition des 8 coins
	c[0] = (t_vec3){box.min.x, box.min.y, box.min.z};
	c[1] = (t_vec3){box.max.x, box.min.y, box.min.z};
	c[2] = (t_vec3){box.max.x, box.max.y, box.min.z};
	c[3] = (t_vec3){box.min.x, box.max.y, box.min.z};
	c[4] = (t_vec3){box.min.x, box.min.y, box.max.z};
	c[5] = (t_vec3){box.max.x, box.min.y, box.max.z};
	c[6] = (t_vec3){box.max.x, box.max.y, box.max.z};
	c[7] = (t_vec3){box.min.x, box.max.y, box.max.z};
	for (i = 0; i < 8; i++)
		p[i] = project_point(data, c[i]);
	//point derriere dessine rien flemme de cliper les lignes
	for (i = 0; i < 8; i++)
	{
		if (!p[i].z_valid)
			return;
	}
	//avant
	draw_line(data, p[0], p[1], color);
	draw_line(data, p[1], p[2], color);
	draw_line(data, p[2], p[3], color);
	draw_line(data, p[3], p[0], color);
	// arrière
	draw_line(data, p[4], p[5], color);
	draw_line(data, p[5], p[6], color);
	draw_line(data, p[6], p[7], color);
	draw_line(data, p[7], p[4], color);
	// Connexions face
	draw_line(data, p[0], p[4], color);
	draw_line(data, p[1], p[5], color);
	draw_line(data, p[2], p[6], color);
	draw_line(data, p[3], p[7], color);
}
