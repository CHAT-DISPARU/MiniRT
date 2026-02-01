/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:11:09 by titan             #+#    #+#             */
/*   Updated: 2026/02/01 13:58:59 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	update(void *param)
{
	t_data	*data;
	t_mat4	rot_mtx;
	t_vec3	forward;
	t_vec3	right;
	bool	movded;

	data = (t_data *)param;
	movded = false;
	forward = data->cam.dir;
	right = get_right_vector(forward);
	if (data->key_table[30] && !data->old_key_table[30])
	{
		movded = true;
		data->s_per_pixs = 1;
	}
	if (data->key_table[31] && !data->old_key_table[31])
	{
		movded = true;
		data->s_per_pixs = 3;
	}
	if (data->key_table[32] && !data->old_key_table[32])
	{
		movded = true;
		data->s_per_pixs = 5;
	}
	if (data->key_table[33] && !data->old_key_table[33])
	{
		movded = true;
		data->s_per_pixs = S_PER_PIXS;
	}
	if (data->key_table[224] && !data->old_key_table[224])
	{
		if (data->speed == 0.5)
		{
			data->speed = 2;
			data->rot_speed = 0.2;
		}
		else
		{
			data->speed = 0.5;
			data->rot_speed = 0.05;
		}
	}
	if (data->key_table[82])
	{
		mat4_rotate_axis(&rot_mtx, right, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[81])
	{
		mat4_rotate_axis(&rot_mtx, right, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[80])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[79])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[47])
	{
		mat4_rotate_axis(&rot_mtx, data->cam.dir, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		data->cam.up_guide = mat4_mult_vec3(&rot_mtx, data->cam.up_guide, 0.0);
		data->cam.up_guide = vec_normalize(data->cam.up_guide);
		movded = true;
	}
	if (data->key_table[48])
	{
		mat4_rotate_axis(&rot_mtx, data->cam.dir, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		data->cam.up_guide = mat4_mult_vec3(&rot_mtx, data->cam.up_guide, 0.0);
		data->cam.up_guide = vec_normalize(data->cam.up_guide);
		movded = true;
	}
	if (data->key_table[82] || data->key_table[81] || 
		data->key_table[80] || data->key_table[79])
		data->cam.dir = vec_normalize(data->cam.dir);
	if (data->key_table[26])
	{
		data->cam.origin = vec_add(data->cam.origin, vec_scale(forward, data->speed));
		movded = true;
	}
	if (data->key_table[22]) 
	{
		data->cam.origin = vec_sub(data->cam.origin, vec_scale(forward, data->speed));
		movded = true;
	}
	if (data->key_table[4]) 
	{
		data->cam.origin = vec_sub(data->cam.origin, vec_scale(right, data->speed));
		movded = true;
	}
	if (data->key_table[7]) 
	{
		data->cam.origin = vec_add(data->cam.origin, vec_scale(right, data->speed));
		movded = true;
	}
	if (data->key_table[44])
	{
		data->cam.origin.y += data->speed;
		movded = true;
	}
	if (data->key_table[225])
	{
		data->cam.origin.y -= data->speed;
		movded = true;
	}
	if (data->key_table[46])
	{
		data->cam.fov -= 2;
		if (data->cam.fov < 1)
			data->cam.fov = 1;
		data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
		data->view_port.viewport_height = 2.0 * tan(data->view_port.fov_radians / 2.0) * data->view_port.focal_length;
		data->view_port.viewport_width = data->view_port.aspect_ratio * data->view_port.viewport_height;
		movded = true;
	}
	if (data->key_table[45])
	{
		data->cam.fov += 2;
		if (data->cam.fov > 179)
			data->cam.fov = 179;
		data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
		data->view_port.viewport_height = 2.0 * tan(data->view_port.fov_radians / 2.0) * data->view_port.focal_length;
		data->view_port.viewport_width = data->view_port.aspect_ratio * data->view_port.viewport_height;
		movded = true;
	}
	if (movded)
		thread_calls(data);
	ft_memcpy(data->old_key_table, data->key_table, sizeof(data->key_table));
}
