/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:57:53 by titan             #+#    #+#             */
/*   Updated: 2026/01/31 21:58:30 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>


/*
	formule fov equation
	Height = 2×tan(2/FOV​) × FocalLength

	Couleur Amb ​= Couleur Obj ​× (Couleur Amb​ × Ratio Amb​)
	Couleur Diff​ = Couleur Obj ​× (Couleur Light ​× Ratio Light​)
		× (Normal ⋅ Light Dir)
*/
void	render(t_data *data)
{
	int			y;
	int			x;
	int			s;
	double		u;
	double		v;
	t_ray		ray;
	t_vec3      local_dir;
	int			idx;
	t_hit_r		rec;
	t_vec3		color_acc;
	t_vec3		pixel;
	t_vec3		final_color;
	double		inv_width = 1.0 / (data->width - 1);
	double		inv_height = 1.0 / (data->height - 1);

	ft_bzero(&rec, sizeof (t_hit_r));
	t_mat4	cam_matrix = look_at(data->cam.origin, data->cam.dir, data->cam.up_guide);
	t_vec3	cam_origin = mat4_mult_vec3(&cam_matrix, (t_vec3){0,0,0}, 1.0);
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			color_acc = (t_vec3){0, 0, 0};
			s = 0;
			while (s < data->s_per_pixs)
			{
				if (data->s_per_pixs == 1)
				{
					u = ((double)x + 0.5) * inv_width;
					v = ((double)y + 0.5) * inv_height;
				}
				else
				{
					u = ((double)x + rand_double()) * inv_width;
					v = ((double)y + rand_double()) * inv_height;
				}
				local_dir.x = (u * data->view_port.viewport_width) - (data->view_port.viewport_width / 2);
				local_dir.y = (data->view_port.viewport_height / 2) - (v * data->view_port.viewport_height);
				local_dir.z = -data->view_port.focal_length;
				ray.origin = cam_origin;
				ray.dir = mat4_mult_vec3(&cam_matrix, local_dir, 0.0);
				ray.dir = vec_normalize(ray.dir);
				if (hit_someting(data, ray, &rec))
				{
					double	obj_r = rec.color.r / 255.0;
					double	obj_g = rec.color.g / 255.0;
					double	obj_b = rec.color.b / 255.0;
					t_vec3	ambient;
					ambient.x = obj_r * data->a_final.x;
					ambient.y = obj_g * data->a_final.y;
					ambient.z = obj_b * data->a_final.z;
					t_vec3	diffuse;
					diffuse.x = data->light.color.r / 255.0;
					diffuse.y = data->light.color.g / 255.0;
					diffuse.z = data->light.color.b / 255.0;
					if (is_in_shadow(data, &rec, &data->light) == false)
					{
						t_vec3	light_dir = vec_sub(data->light.origin, rec.p);
						light_dir = vec_normalize(light_dir);
						double	diff_strength = vec_dot_scal(rec.normal, light_dir);
						if (diff_strength > 0)
						{
							diffuse.x *= obj_r * data->light.ratio * diff_strength;
							diffuse.y *= obj_g * data->light.ratio * diff_strength;
							diffuse.z *= obj_b * data->light.ratio * diff_strength;
						}
						else
							diffuse = (t_vec3){0, 0, 0};
					}
					else
						diffuse = (t_vec3){0, 0, 0};
					pixel = vec_add(ambient, diffuse);
					color_acc = vec_add(color_acc, pixel);
				}
				s++;
			}
			final_color = vec_scale(color_acc, 1.0 / data->s_per_pixs);
			if (final_color.x > 1.0)
				final_color.x = 1.0;
			if (final_color.y > 1.0)
				final_color.y = 1.0;
			if (final_color.z > 1.0)
				final_color.z = 1.0;
			int ir = (int)(255.999 * final_color.x);
			int ig = (int)(255.999 * final_color.y);
			int ib = (int)(255.999 * final_color.z);
			idx = y * data->width + x;
			data->pixels[idx].rgba = (ir << 24) | (ig << 16) | (ib << 8) | 0xFF;
			x++;
		}
		y++;
	}
	mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
