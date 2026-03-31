/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_calls_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 13:59:22 by titan             #+#    #+#             */
/*   Updated: 2026/03/31 18:13:13 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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

void	send_task(t_data *data, t_thread_c_int *utils,
		int indexs[THREADS_COUNT*NB_TASK_R])
{
	t_thread_info	infos;
	t_net_task		net_task;
	t_net_header	header;
	int				worker_target;

	header.type = MSG_TASK;
	header.size = sizeof(t_net_task);
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
		worker_target = utils->i % (data->client_count + 1);
		if (worker_target < data->client_count)
		{
			net_task.task_id = utils->i;
			net_task.start_x = infos.start_x;
			net_task.end_x   = infos.end_x;
			net_task.start_y = infos.start_y;
			net_task.end_y   = infos.end_y;
			send_all(data->client_sockets[worker_target], &header, sizeof(t_net_header));
			send_all(data->client_sockets[worker_target], &net_task, sizeof(t_net_task));
		}
		else
			add_task(data, render, infos);
		utils->i++;
	}
	for (int k = 0; k < data->client_count; k++)
	{
		header.type = MSG_END_TASKS;
		send_all(data->client_sockets[k], &header, sizeof(t_net_header));
	}
}

void	thread_calls(t_data *data)
{
	int				indexs[THREADS_COUNT * NB_TASK_R];
	t_thread_c_int	utils;
	int				i;

	prepare_calls(data, &utils);
	set_indexs(indexs);
	i = 0;
	while (i < data->client_count) 
	{
		if (send_full_scene(data->client_sockets[i], data) == -1)
			ft_putstr_fd("error scene envoie\n", 2);
		i++;
	}
	send_task(data, &utils, indexs);
	while (1)
	{
		pthread_mutex_lock(&data->finish_count);
		utils.finish = data->finish;
		pthread_mutex_unlock(&data->finish_count);
		print_progress(utils.finish, THREADS_COUNT * NB_TASK_R);
		if (utils.finish == THREADS_COUNT * NB_TASK_R)
			break ;
		fd_set  readfds;
		int     max_sd = 0;
		int     sd;
		
		FD_ZERO(&readfds);
		for (int k = 0; k < data->client_count; k++)
		{
			sd = data->client_sockets[k];
			if (sd > 0)
			{
				FD_SET(sd, &readfds);
				if (sd > max_sd)
					max_sd = sd;
			}
		}
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 200;
		int activity = select(max_sd + 1, &readfds, NULL, NULL, &tv);
		if (activity > 0)
		{
			for (int k = 0; k < data->client_count; k++)
			{
				sd = data->client_sockets[k];
				if (sd > 0 && FD_ISSET(sd, &readfds))
				{
					t_net_header header;
					
					if (recv_all(sd, &header, sizeof(t_net_header)) == 0)
					{
						if (header.type == MSG_PIXELS)
						{
							recv_task_result(sd, data, header.size);
							pthread_mutex_lock(&data->finish_count);
							data->finish++;
							pthread_mutex_unlock(&data->finish_count);
						}
					}
					else
					{
						ft_putstr_fd("\nUn client s'est deconnecte.\n", 2);
						close(sd);
						data->client_sockets[k] = 0;
					}
				}
			}
		}
	}
	pthread_mutex_destroy(&data->finish_count);
	draw_lines(data, utils.grid_w, utils.grid_h);
	mlx_set_image_region(data->mlx, data->img, 0, 0,
		data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
