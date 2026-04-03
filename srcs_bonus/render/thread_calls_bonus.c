/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_calls_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: CHAT-DISPARU <CHAT-DISPARU@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 13:59:22 by titan             #+#    #+#             */
/*   Updated: 2026/04/04 01:02:51 by CHAT-DISPAR      ###   ########.fr       */
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

void	prepare_calls(t_data *data, t_thread_c_int *utils, int client_count)
{
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	utils->cols = sqrt(THREADS_COUNT * NB_TASK_R * client_count);
	while (THREADS_COUNT * NB_TASK_R * client_count % utils->cols != 0)
		utils->cols--;
	utils->rows = THREADS_COUNT * NB_TASK_R * client_count / utils->cols;
	utils->grid_w = data->width / utils->cols;
	utils->grid_h = data->height / utils->rows;
	utils->i = 0;
	data->finish = 0;
	pthread_mutex_init(&data->finish_count, NULL);
	pthread_mutex_lock(&data->finish_count);
	utils->finish = data->finish;
	pthread_mutex_unlock(&data->finish_count);
	print_progress(utils->finish, THREADS_COUNT * NB_TASK_R * client_count);
}

int send_task(t_data *data, int target_sock, t_thread_c_int *utils, int *indexs, int total_tasks, int *next_task, int batch_size)
{
	t_net_header	header;
	t_net_task		net_task;
	t_thread_info	infos;
	int				sent = 0;

	header.type = MSG_TASK;
	header.size = sizeof(t_net_task);
	while (sent < batch_size && *next_task < total_tasks)
	{
		int idx = indexs[*next_task];
		utils->current_col = idx % utils->cols;
		utils->current_row = idx / utils->cols;

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
		if (target_sock > 0)
		{
			net_task.task_id = *next_task;
			net_task.start_x = infos.start_x;
			net_task.end_x   = infos.end_x;
			net_task.start_y = infos.start_y;
			net_task.end_y   = infos.end_y;
			send_all(target_sock, &header, sizeof(t_net_header));
			send_all(target_sock, &net_task, sizeof(t_net_task));
		}
		else
			add_task(data, render, infos);
		
		(*next_task)++;
		sent++;
	}
	if (target_sock > 0 && sent > 0)
	{
		header.type = MSG_END_TASKS;
		header.size = sizeof(t_net_header);
		send_all(target_sock, &header, sizeof(t_net_header));
	}
	return (sent);
}

void	thread_calls(t_data *data)
{
	int				*indexs;
	t_thread_c_int	utils;
	int				client_count = data->client_count > 0 ? data->client_count + 1 : 1;
	
	int             total_tasks = THREADS_COUNT * NB_TASK_R * client_count * 8;
	prepare_calls(data, &utils, client_count * 8);
	indexs = malloc(sizeof(int) * total_tasks);
	set_indexs(indexs, total_tasks);
	for (int i = 0; i < data->client_count; i++) 
	{
		if (send_scene_low(data->client_sockets[i], data) == -1)
			ft_putstr_fd("error scene envoie\n", 2);
	}
	int	next_task = 0;
	int	client_pending[CLIENT_MAX] = {0};
	int	local_assigned = 0;
	int	remote_finished = 0;
	int	batch_size = THREADS_COUNT;
	int frame_baseline = 0;
    pthread_mutex_lock(&data->finish_count);
    frame_baseline = data->finish;
    pthread_mutex_unlock(&data->finish_count);
	for (int k = 0; k < data->client_count; k++)
		client_pending[k] = send_task(data, data->client_sockets[k], &utils, indexs, total_tasks, &next_task, batch_size);
	local_assigned += send_task(data, -1, &utils, indexs, total_tasks, &next_task, batch_size);
	while (1)
	{
		pthread_mutex_lock(&data->finish_count);
		utils.finish = data->finish;
		pthread_mutex_unlock(&data->finish_count);
		
		int frame_finish = utils.finish - frame_baseline;
		int	local_finished = frame_finish - remote_finished;
		if (local_assigned - local_finished < THREADS_COUNT && next_task < total_tasks)
			local_assigned += send_task(data, -1, &utils, indexs, total_tasks, &next_task, batch_size);
		print_progress(frame_finish, total_tasks);
		if (frame_finish >= total_tasks)
			break ;
		fd_set	readfds;
		int		max_sd = 0;
		int		sd;
		
		FD_ZERO(&readfds);
		for (int k = 0; k < data->client_count; k++)
		{
			sd = data->client_sockets[k];
			if (sd > 0)
			{
				FD_SET(sd, &readfds);
				if (sd > max_sd) max_sd = sd;
			}
		}
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 1000;
		if (select(max_sd + 1, &readfds, NULL, NULL, &tv) > 0)
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
							
							remote_finished++;
							client_pending[k]--;
							if (client_pending[k] == 0 && next_task < total_tasks)
							{
								client_pending[k] = send_task(data, sd, &utils, indexs, total_tasks, &next_task, batch_size);
							}
						}
					}
					else
					{
						ft_putstr_fd("\nUn client c'est deco.\n", 2);
						close(sd);
						data->client_sockets[k] = 0;
					}
				}
			}
		}
		//mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
		//mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	}
	draw_lines(data, utils.grid_w, utils.grid_h);
	mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	free(indexs);
}
