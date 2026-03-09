/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_calls_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 13:59:22 by titan             #+#    #+#             */
/*   Updated: 2026/03/09 17:47:40 by gajanvie         ###   ########.fr       */
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

void	draw_lines2(t_data *data, int grid_w, int grid_h, int y)
{
	t_vec2int	idxs;

	while (y < data->height)
	{
		idxs.x = 0;
		while (idxs.x < data->width)
		{
			idxs.y = (y) * data->width + (idxs.x);
			data->pixels[idxs.y] = (mlx_color)(uint32_t){0xFF0000FF};
			idxs.x += grid_w;
		}
		y++;
	}
	idxs.x = 0;
	while (idxs.x < data->width)
	{
		y = 0;
		while (y < data->height)
		{
			idxs.y = (y) * data->width + (idxs.x);
			data->pixels[idxs.y] = (mlx_color)(uint32_t){0xFF0000FF};
			y += grid_h;
		}
		idxs.x++;
	}
}

void	draw_lines(t_data *data, int grid_w, int grid_h)
{
	if (data->lines)
	{
		if (grid_w == 0)
			grid_w = 1;
		if (grid_h == 0)
			grid_h = 1;
		draw_lines2(data, grid_w, grid_h, 0);
	}
}

void	prepare_calls(t_data *data, t_thread_c_int *utils)
{
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	utils->cols = sqrt(THREADS_COUNT * NB_TASK_R);
	while (THREADS_COUNT * NB_TASK_R % utils->cols != 0)
		utils->cols--;
	utils->rows = THREADS_COUNT * NB_TASK_R / utils->cols;
	utils->grid_w = data->width / utils->cols;
	utils->grid_h = data->height / utils->rows;
	utils->i = 0;
	data->finish = 0;
	pthread_mutex_init(&data->finish_count, NULL);
	pthread_mutex_lock(&data->finish_count);
	utils->finish = data->finish;
	pthread_mutex_unlock(&data->finish_count);
	print_progress(utils->finish, THREADS_COUNT * NB_TASK_R);
}

void	send_task(t_data *data,
	t_thread_c_int *utils, int indexs[THREADS_COUNT * NB_TASK_R])
{
	t_thread_info	infos;

	while (utils->i < THREADS_COUNT * NB_TASK_R)
	{
		utils->current_col = indexs[utils->i] % utils->cols;
		utils->current_row = indexs[utils->i] / utils->cols;
		infos.data = data;
		infos.start_x = utils->current_col * utils->grid_w;
		if (utils->current_col == utils->cols - 1)
			infos.end_x = data->width;
		else
			infos.end_x = (utils->current_col + 1) * utils->grid_w;
		infos.start_y = utils->current_row * utils->grid_h;
		if (utils->current_row == utils->rows - 1)
			infos.end_y = data->height;
		else
			infos.end_y = (utils->current_row + 1) * utils->grid_h;
		add_task(data, render, infos);
		utils->i++;
	}
}

void	thread_calls(t_data *data)
{
	int				indexs[THREADS_COUNT * NB_TASK_R];
	t_thread_c_int	utils;

	prepare_calls(data, &utils);
 	set_indexs(indexs);
	send_task(data, &utils, indexs);
	while (1)
	{
		pthread_mutex_lock(&data->finish_count);
		utils.finish = data->finish;
		pthread_mutex_unlock(&data->finish_count);
		print_progress(utils.finish, THREADS_COUNT * NB_TASK_R);
		if (utils.finish == THREADS_COUNT * NB_TASK_R)
			break ;
		usleep(200);
	}
	pthread_mutex_destroy(&data->finish_count);
	draw_lines(data, utils.grid_w, utils.grid_h);
	mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
