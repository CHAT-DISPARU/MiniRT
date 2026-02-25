/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 11:47:14 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/02 11:58:59 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

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
