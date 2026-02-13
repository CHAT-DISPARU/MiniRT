/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:37:35 by titan             #+#    #+#             */
/*   Updated: 2026/02/12 20:09:16 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	display_fps(t_data *data)
{
	static char	buf[128];
	double		current_time;
	double		delta_time;
	int			fps;

	current_time = get_time();
	delta_time = current_time - data->last_frame_time;
	data->last_frame_time = current_time;
	fps = (delta_time > 0) ? (int)(1.0 / delta_time) : 999;
	sprintf(buf, "FPS: %d | STEP: %d | SAMPLES: %d | DEPTH: %d", fps, data->step, data->s_per_pixs, data->deph);
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