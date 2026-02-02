/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:57:53 by titan             #+#    #+#             */
/*   Updated: 2026/02/02 11:31:10 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

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

void	check_hit(t_data *data, t_ray ray, t_vec3 *color_acc)
{
	t_color_c	lights;
	double		diff_strength;
	t_vec3		light_dir;
	t_hit_r		rec;

	ft_bzero(&rec, sizeof (t_hit_r));
	if (hit_someting(data, ray, &rec))
	{
		calc_lights(&lights, rec, data);
		if (is_in_shadow(data, &rec, &data->light) == false)
		{
			light_dir = vec_sub(data->light.origin, rec.p);
			light_dir = vec_normalize(light_dir);
			diff_strength = vec_dot_scal(rec.normal, light_dir);
			if (diff_strength > 0)
				final_diffuse(&lights, data, diff_strength);
			else
				lights.diffuse = (t_vec3){0, 0, 0};
		}
		else
			lights.diffuse = (t_vec3){0, 0, 0};
		*color_acc = vec_add(*color_acc,
				vec_add(lights.ambient, lights.diffuse));
	}
}

void	do_samples(t_data *data, t_idxs idxs, t_render_v rv, t_vec3 *color_acc)
{
	t_ray	ray;

	idxs.s = 0;
	while (idxs.s < data->s_per_pixs)
	{
		if (data->s_per_pixs == 1)
		{
			rv.u = ((double)idxs.x + 0.5) * rv.inv_width;
			rv.v = ((double)idxs.y + 0.5) * rv.inv_height;
		}
		else
		{
			rv.u = ((double)idxs.x + rand_double()) * rv.inv_width;
			rv.v = ((double)idxs.y + rand_double()) * rv.inv_height;
		}
		ray = calc_ray(rv, data);
		check_hit(data, ray, color_acc);
		idxs.s++;
	}
}

void	set_final_color(t_vec3 color_acc, t_data *data, t_idxs idxs)
{
	int			idx;
	t_vec3		final_color;
	t_ic		ic;

	final_color = vec_scale(color_acc, 1.0 / data->s_per_pixs);
	if (final_color.x > 1.0)
		final_color.x = 1.0;
	if (final_color.y > 1.0)
		final_color.y = 1.0;
	if (final_color.z > 1.0)
		final_color.z = 1.0;
	final_color.x = pow(final_color.x, 0.4545);
	final_color.y = pow(final_color.y, 0.4545);
	final_color.z = pow(final_color.z, 0.4545);
	ic.ir = (int)(255.999 * final_color.x);
	ic.ig = (int)(255.999 * final_color.y);
	ic.ib = (int)(255.999 * final_color.z);
	idx = idxs.y * data->width + idxs.x;
	data->pixels[idx].rgba = (ic.ir << 24)
		| (ic.ig << 16) | (ic.ib << 8) | 0xFF;
}

/*
	formule fov equation
	Height = 2×tan(2/FOV​) × FocalLength

	Couleur Amb ​= Couleur Obj ​× (Couleur Amb​ × Ratio Amb​)
	Couleur Diff​ = Couleur Obj ​× (Couleur Light ​× Ratio Light​)
		× (Normal ⋅ Light Dir)
*/
void	render(t_data *data)
{
	t_idxs		idxs;
	t_vec3		color_acc;
	t_render_v	rv;

	rv.inv_width = 1.0 / (data->width - 1);
	rv.inv_height = 1.0 / (data->height - 1);
	rv.cam_m = look_at(data->cam.origin, data->cam.dir, data->cam.up_guide);
	rv.cam_origin = mat4_mult_vec3(&rv.cam_m, (t_vec3){0, 0, 0}, 1.0);
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	idxs.y = 0;
	while (idxs.y < data->height)
	{
		idxs.x = 0;
		while (idxs.x < data->width)
		{
			color_acc = (t_vec3){0, 0, 0};
			do_samples(data, idxs, rv, &color_acc);
			set_final_color(color_acc, data, idxs);
			idxs.x++;
		}
		idxs.y++;
	}
	mlx_set_image_region(data->mlx, data->img, 0, 0,
		data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
