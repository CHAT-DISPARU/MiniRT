/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:11:09 by titan             #+#    #+#             */
/*   Updated: 2026/03/11 10:57:58 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	change_speed(t_data *data, bool *movded)
{
	if (data->key_table[224] && !data->old_key_table[224])
	{
		if (data->speed == 0.3)
		{
			data->speed = 4;
			data->rot_speed = 0.1;
		}
		else
		{
			data->speed = 0.3;
			data->rot_speed = 0.05;
		}
	}
	if (data->key_table[58] && !data->old_key_table[58])
	{
		if (data->step > 1)
			data->step--;
		*movded = true;
	}
	if (data->key_table[59] && !data->old_key_table[59])
	{
		data->step++;
		*movded = true;
	}
}

void	update_fov2(bool *movded, t_data *data)
{
	if (data->key_table[45])
	{
		data->cam.fov += 2;
		if (data->cam.fov > 179)
			data->cam.fov = 179;
		data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
		data->view_port.viewport_height = 2.0 * tan(data->view_port.fov_radians
				/ 2.0) * data->view_port.focal_length;
		data->view_port.viewport_width = data->view_port.aspect_ratio
			* data->view_port.viewport_height;
		*movded = true;
	}
}

void	update_fov(bool *movded, t_data *data)
{
	if (data->key_table[42] && !data->old_key_table[42])
	{
		data->diff_ok = !data->diff_ok;
		*movded = true;
	}
	if (data->key_table[46])
	{
		data->cam.fov -= 2;
		if (data->cam.fov < 1)
			data->cam.fov = 1;
		data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
		data->view_port.viewport_height = 2.0 * tan(data->view_port.fov_radians
				/ 2.0) * data->view_port.focal_length;
		data->view_port.viewport_width = data->view_port.aspect_ratio
			* data->view_port.viewport_height;
		*movded = true;
	}
	update_fov2(movded, data);
}

void	update2(bool *movded, t_data *data)
{
	if (data->key_table[5] && !data->old_key_table[5])
	{
		data->debug = !data->debug;
		*movded = true;
	}
	if (data->key_table[40] && !data->old_key_table[40])
	{
		data->use_bvh = !data->use_bvh;
		*movded = true;
	}
	if (data->key_table[6] && !data->old_key_table[6])
	{
		data->has_checker = !data->has_checker;
		*movded = true;
	}
	if (data->key_table[269] && !data->old_key_table[269])
	{
		mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
		mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	}
}

void	update(void *param)
{
	t_data	*data;
	t_vec3	forward;
	t_vec3	right;
	bool	movded;

	data = (t_data *)param;
	movded = false;
	forward = data->cam.dir;
	right = get_right_vector(forward);
	update_samples(data, &movded);
	change_speed(data, &movded);
	update_rot(data, right, &movded);
	update_move(data, &movded, forward, right);
	update_fov(&movded, data);
	update2(&movded, data);
	relaunch(&movded, data);
	if (data->key_table[15] && !data->old_key_table[15])
		data->lines = !data->lines;
	if (data->key_table[15] && !data->old_key_table[15])
		movded = true;
	if (movded)
		thread_calls(data);
	if (movded)
		display_fps(data);
	ft_memcpy(data->old_key_table, data->key_table, sizeof(data->key_table));
}
