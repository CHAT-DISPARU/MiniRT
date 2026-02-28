/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:11:09 by titan             #+#    #+#             */
/*   Updated: 2026/02/28 14:23:15 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	update_samples(t_data *data, bool *movded)
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

void	change_speed(t_data *data, bool *movded)
{
	if (data->key_table[224] && !data->old_key_table[224])
	{
		if (data->speed == 0.5)
		{
			data->speed = 2;
			data->rot_speed = 0.1;
		}
		else
		{
			data->speed = 0.5;
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
	if (data->key_table[5] && !data->old_key_table[5])
	{
		data->debug = !data->debug;
		movded = true;
	}
	if (data->key_table[40] && !data->old_key_table[40])
	{
		data->use_bvh = !data->use_bvh;
		movded = true;
	}
	if (data->key_table[6] && !data->old_key_table[6])
	{
		data->has_checker = !data->has_checker;
		movded = true;
	}
	if (data->key_table[269] && !data->old_key_table[269])
	{
		mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
		mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	}
	if (data->key_table[229] && !data->old_key_table[229])
	{
		clean(data);
		re_init(data);
		read_rt(data);
		convert_list_to_arrays(data);
		calcul_ambient(data);
		movded = true;
	}
	if (data->key_table[15] && !data->old_key_table[15])
	{
		data->lines = !data->lines;
		movded = true;
	}
	if (movded)
		thread_calls(data);
	if (movded)
		display_fps(data);
	ft_memcpy(data->old_key_table, data->key_table, sizeof(data->key_table));
}
