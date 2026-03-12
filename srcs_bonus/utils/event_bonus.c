/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:56:43 by titan             #+#    #+#             */
/*   Updated: 2026/02/28 13:21:37 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	key_up(int key, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (key >= 0 && key < 512)
		data->key_table[key] = 0;
}

void	key_down(int key, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (key == 41)
		mlx_loop_end(data->mlx);
	if (key >= 0 && key < 512)
		data->key_table[key] = 1;
}

void	window_hook(int event, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (event == 0)
		mlx_loop_end(data->mlx);
	else if (event == 1)
	{
		if (resize_win(data) != 0)
			mlx_loop_end(data->mlx);
	}
}
