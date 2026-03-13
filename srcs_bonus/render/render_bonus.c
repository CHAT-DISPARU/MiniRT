/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:57:53 by titan             #+#    #+#             */
/*   Updated: 2026/03/13 09:43:55 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	do_samples(t_data *data, t_idxs idxs, t_render_v rv, t_vec3 *color_acc)
{
	t_ray	ray;
	t_vec3	sample_color;

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
		sample_color = check_hit(data, ray, data->deph);
		*color_acc = vec_add(*color_acc, sample_color);
		idxs.s++;
	}
}

void	draw_step(t_ic ic, t_data *data, t_idxs idxs, t_thread_info *info)
{
	int			idx;
	int			y1;
	int			x1;
	t_vec2int	vec;

	vec.y = 0;
	while (vec.y < data->step)
	{
		vec.x = 0;
		y1 = idxs.y + vec.y;
		if (y1 >= info->end_y)
			break ;
		while (vec.x < data->step)
		{
			x1 = idxs.x + vec.x;
			if (x1 >= info->end_x)
				break ;
			idx = (y1) * data->width + (x1);
			data->pixels[idx].rgba = (ic.ir << 24)
				| (ic.ig << 16) | (ic.ib << 8) | 0xFF;
			vec.x++;
		}
		vec.y++;
	}
}

void	set_final_color(t_vec3 color_acc,
	t_data *data, t_idxs idxs, t_thread_info *info)
{
	t_vec3		final_color;
	t_ic		ic;

	final_color = vec_scale(color_acc, 1.0 / data->s_per_pixs);
	if (final_color.x > 1.0)
		final_color.x = 1.0;
	if (final_color.y > 1.0)
		final_color.y = 1.0;
	if (final_color.z > 1.0)
		final_color.z = 1.0;
	final_color.x = pow(final_color.x, 0.45454545454);
	final_color.y = pow(final_color.y, 0.45454545454);
	final_color.z = pow(final_color.z, 0.45454545454);
	ic.ir = (int)(255.999 * final_color.x);
	ic.ig = (int)(255.999 * final_color.y);
	ic.ib = (int)(255.999 * final_color.z);
	draw_step(ic, data, idxs, info);
}

void	add_finish(t_data *data)
{
	pthread_mutex_lock(&data->finish_count);
	data->finish++;
	pthread_mutex_unlock(&data->finish_count);
}

/*
	Couleur Amb ​= Couleur Obj ​× (Couleur Amb​ × Ratio Amb​)
	Couleur Diff​ = Couleur Obj ​× (Couleur Light ​× Ratio Light​)
		× (Normal ⋅ Light Dir)

	couleur spec ​=C light​⋅ks​⋅(max(0,V⋅R)) exposant α
*/
void	render(void *arg)
{
	t_thread_info	*info;
	t_data			*data;
	t_vec3			color_acc;
	t_render_v		rv;

	info = (t_thread_info *)arg;
	data = info->data;
	rv.inv_width = 1.0 / (data->width - 1);
	rv.inv_height = 1.0 / (data->height - 1);
	rv.cam_m = look_at(data->cam.origin, data->cam.dir, data->cam.up_guide);
	rv.cam_origin = mat4_mult_vec3(&rv.cam_m, (t_vec3){0, 0, 0}, 1.0);
	rv.idxs.y = info->start_y;
	while (rv.idxs.y < info->end_y)
	{
		rv.idxs.x = info->start_x;
		while (rv.idxs.x < info->end_x)
		{
			color_acc = (t_vec3){0, 0, 0};
			do_samples(data, rv.idxs, rv, &color_acc);
			set_final_color(color_acc, data, rv.idxs, info);
			rv.idxs.x += data->step;
		}
		rv.idxs.y += data->step;
	}
	add_finish(data);
}
