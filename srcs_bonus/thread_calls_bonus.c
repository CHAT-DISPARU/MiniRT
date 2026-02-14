/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_calls_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 13:59:22 by titan             #+#    #+#             */
/*   Updated: 2026/02/14 18:41:01 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	thread_calls(t_data *data)
{
	pthread_t		threads[THREADS_COUNT];
	t_thread_info	infos[THREADS_COUNT];
	int				i;
	int				cols;
	int				rows;
	int				grid_w;
	int				grid_h;
	int				current_col;
	int				current_row;

	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	cols = sqrt(THREADS_COUNT);
	while (THREADS_COUNT % cols != 0)
		cols--;
	rows = THREADS_COUNT / cols;
	grid_w = data->width / cols;
	grid_h = data->height / rows;
	i = 0;
	while (i < THREADS_COUNT)
	{
		current_col = i % cols;
		current_row = i / cols;
		infos[i].data = data;
		infos[i].start_x = current_col * grid_w;
		if (current_col == cols - 1)
			infos[i].end_x = data->width;
		else
			infos[i].end_x = (current_col + 1) * grid_w;
		infos[i].start_y = current_row * grid_h;
		if (current_row == rows - 1)
			infos[i].end_y = data->height;
		else
			infos[i].end_y = (current_row + 1) * grid_h;
		if (pthread_create(&threads[i], NULL, render, &infos[i]))
			clean_exit(data, 1, "phtread create error\n", 0);
		i++;
	}
	i = 0;
	while (i < THREADS_COUNT)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
