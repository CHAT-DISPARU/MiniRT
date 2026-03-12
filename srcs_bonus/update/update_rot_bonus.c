/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rot_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 11:47:14 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:56:35 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	update_samples2(t_data *data, bool *movded)
{
	if (data->key_table[30] && !data->old_key_table[30])
	{
		*movded = true;
		data->s_per_pixs = 1;
	}
	if (data->key_table[31] && !data->old_key_table[31])
	{
		*movded = true;
		data->s_per_pixs = 5;
	}
	if (data->key_table[32] && !data->old_key_table[32])
	{
		*movded = true;
		data->s_per_pixs = 10;
	}
}

void	update_samples(t_data *data, bool *movded)
{
	update_samples2(data, movded);
	if (data->key_table[33] && !data->old_key_table[33])
	{
		*movded = true;
		data->s_per_pixs = 30;
	}
	if (data->key_table[34] && !data->old_key_table[34])
	{
		*movded = true;
		data->s_per_pixs = S_PER_PIXS;
	}
	if (data->key_table[39] && !data->old_key_table[39])
	{
		*movded = true;
		data->deph++;
	}
	if (data->key_table[38] && !data->old_key_table[38] && data->deph > 1)
	{
		*movded = true;
		data->deph--;
	}
}

void	update_move(t_data *data, bool *movded, t_vec3 forward, t_vec3 right)
{
	if (data->key_table[26])
		data->cam.origin = vec_add(data->cam.origin,
				vec_scale(forward, data->speed));
	if (data->key_table[22])
		data->cam.origin = vec_sub(data->cam.origin,
				vec_scale(forward, data->speed));
	if (data->key_table[4])
		data->cam.origin = vec_sub(data->cam.origin,
				vec_scale(right, data->speed));
	if (data->key_table[7])
		data->cam.origin = vec_add(data->cam.origin,
				vec_scale(right, data->speed));
	if (data->key_table[44])
		data->cam.origin.y += data->speed;
	if (data->key_table[225])
		data->cam.origin.y -= data->speed;
	if (data->key_table[26] || data->key_table[22] || data->key_table[4]
		|| data->key_table[7] || data->key_table[44] || data->key_table[225])
		*movded = true;
}

void	update_rot2(t_data *data, t_mat4 rot_mtx, bool *movded)
{
	if (data->key_table[82] || data->key_table[81] || data->key_table[80]
		|| data->key_table[79])
		*movded = true;
	if (data->key_table[47])
	{
		mat4_rotate_axis(&rot_mtx, data->cam.dir, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		data->cam.up_guide = mat4_mult_vec3(&rot_mtx, data->cam.up_guide, 0.0);
		data->cam.up_guide = vec_normalize(data->cam.up_guide);
		*movded = true;
	}
	if (data->key_table[48])
	{
		mat4_rotate_axis(&rot_mtx, data->cam.dir, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		data->cam.up_guide = mat4_mult_vec3(&rot_mtx, data->cam.up_guide, 0.0);
		data->cam.up_guide = vec_normalize(data->cam.up_guide);
		*movded = true;
	}
	if (data->key_table[82] || data->key_table[81]
		|| data->key_table[80] || data->key_table[79]
		|| data->key_table[47] || data->key_table[48])
		data->cam.dir = vec_normalize(data->cam.dir);
}

void	update_rot(t_data *data, t_vec3 right, bool *movded)
{
	t_mat4	rot_mtx;

	if (data->key_table[82])
	{
		mat4_rotate_axis(&rot_mtx, right, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
	}
	if (data->key_table[81])
	{
		mat4_rotate_axis(&rot_mtx, right, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
	}
	if (data->key_table[80])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
	}
	if (data->key_table[79])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
	}
	update_rot2(data, rot_mtx, movded);
}
