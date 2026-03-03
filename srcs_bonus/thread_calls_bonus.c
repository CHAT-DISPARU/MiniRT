/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_calls_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 13:59:22 by titan             #+#    #+#             */
/*   Updated: 2026/03/01 21:48:19 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_indexs(int *indexs)
{
	int	i;
	int	tmp;
	int	j;

	i = 0;
	while (i < THREADS_COUNT * NB_TASK_R)
	{
		indexs[i] = i;
		i++;
	}
	i = THREADS_COUNT * NB_TASK_R - 1;
	while (i > 0)
	{
		j = rand() % (i + 1); 
		tmp = indexs[i];
		indexs[i] = indexs[j];
		indexs[j] = tmp;
		i--;
	}
}

void	thread_calls(t_data *data)
{
	t_thread_info	infos;
	int				indexs[THREADS_COUNT * NB_TASK_R];
	int				i;
	int				x;
	int				y;
	int				cols;
	int				idx;
	int				rows;
	int				grid_w;
	int				finish;
	int				grid_h;
	int				current_col;
	int				current_row;

	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	cols = sqrt(THREADS_COUNT * NB_TASK_R);
	while (THREADS_COUNT * NB_TASK_R % cols != 0)
		cols--;
	rows = THREADS_COUNT * NB_TASK_R / cols;
	grid_w = data->width / cols;
	grid_h = data->height / rows;
	i = 0;
	data->finish = 0;
	set_indexs(indexs);
	pthread_mutex_init(&data->finish_count, NULL);
	pthread_mutex_lock(&data->finish_count);
	finish = data->finish;
	pthread_mutex_unlock(&data->finish_count);
	print_progress(finish, THREADS_COUNT * NB_TASK_R);
	while (i < THREADS_COUNT * NB_TASK_R)
	{
		current_col = indexs[i] % cols;
		current_row = indexs[i] / cols;
		infos.data = data;
		infos.start_x = current_col * grid_w;
		if (current_col == cols - 1)
			infos.end_x = data->width;
		else
			infos.end_x = (current_col + 1) * grid_w;
		infos.start_y = current_row * grid_h;
		if (current_row == rows - 1)
			infos.end_y = data->height;
		else
			infos.end_y = (current_row + 1) * grid_h;
		add_task(data, render, infos);
		i++;
	}
	while (1)
	{
		pthread_mutex_lock(&data->finish_count);
		finish = data->finish;
		pthread_mutex_unlock(&data->finish_count);
		print_progress(finish, THREADS_COUNT * NB_TASK_R);
		if (finish == THREADS_COUNT * NB_TASK_R)
			break ;
		usleep(200);
	}
	pthread_mutex_destroy(&data->finish_count);
	if (data->lines)
	{
		if (grid_w == 0)
			grid_w = 1;
		if (grid_h == 0)
			grid_h = 1;
		y = 0;
		while (y < data->height)
		{
			x = 0;
			while (x < data->width)
			{
				idx = (y) * data->width + (x);
				data->pixels[idx] = (mlx_color)(uint32_t){0xFF0000FF};
				x += grid_w;
			}
			y++;
		}
		x = 0;
		while (x < data->width)
		{
			y = 0;
			while (y < data->height)
			{
				idx = (y) * data->width + (x);
				data->pixels[idx] = (mlx_color)(uint32_t){0xFF0000FF};
				y += grid_h;
			}
			x++;
		}
	}
	mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
