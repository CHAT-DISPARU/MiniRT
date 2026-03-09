/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:37:35 by titan             #+#    #+#             */
/*   Updated: 2026/03/09 16:38:03 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	print_info(int fps, t_data *data, char string)
{
	static char	buf[128];

	sprintf(buf, "FPS: %d | STEP: %d | SAMPLES: %d | DEPTH: %d | DIF: %c", fps, data->step, data->s_per_pixs, data->deph, string);
	mlx_string_put(data->mlx, data->win, 10, 20, (mlx_color)(uint32_t){0xFF0000FF}, buf);
	sprintf(buf, "POS: X:%.2f Y:%.2f Z:%.2f", 
			data->cam.origin.x, data->cam.origin.y, data->cam.origin.z);
	mlx_string_put(data->mlx, data->win, 10, 40, (mlx_color)(uint32_t){0xFFFFFFFF}, buf);
	sprintf(buf, "DIR: X:%.2f Y:%.2f Z:%.2f", 
			data->cam.dir.x, data->cam.dir.y, data->cam.dir.z);
	mlx_string_put(data->mlx, data->win, 10, 60, (mlx_color)(uint32_t){0xC8C8C8FF}, buf);
	if (data->use_bvh)
		mlx_string_put(data->mlx, data->win, 10, 80, (mlx_color)(uint32_t){0x00FF00FF}, "BVH: ACTIVE");
	else
		mlx_string_put(data->mlx, data->win, 10, 80, (mlx_color)(uint32_t){0xFF0000FF}, "BVH: DISABLED");
}

void	display_fps(t_data *data)
{
	double		current_time;
	double		delta_time;
	int			fps;
	char		string;

	if (data->diff_ok)
		string = 't';
	else
		string = 'f';
	current_time = get_time();
	delta_time = current_time - data->last_frame_time;
	data->last_frame_time = current_time;
	if (delta_time > 0)
		fps = (int)(1.0 / delta_time);
	else
		fps = 999;
	print_info(fps, data, string);
}
