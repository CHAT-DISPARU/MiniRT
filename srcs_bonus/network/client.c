/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:18:46 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/31 17:54:02 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	send_task_results(int sock, t_data *data, t_net_task *tasks, int task_count)
{
	t_net_header	header;
	t_net_result	result;
	uint32_t		pixel_count;
	uint32_t		pixels_size;
	mlx_color		*task_pixels;
	int				i;
	int				x;
	int				y;

	i = 0;
	while (i < task_count)
	{
		result.task_id = tasks[i].task_id;
		result.start_x = tasks[i].start_x;
		result.start_y = tasks[i].start_y;
		result.width = tasks[i].end_x - tasks[i].start_x;
		result.height = tasks[i].end_y - tasks[i].start_y;    
		pixel_count = result.width * result.height;
		pixels_size = pixel_count * sizeof(mlx_color);
		task_pixels = malloc(pixels_size);
		if (!task_pixels)
		{
			ft_putstr_fd("Erreur malloc task_pixels\n", 2);
			return ; 
		}
		y = 0;
		while (y < result.height)
		{
			x = 0;
			while (x < result.width)
			{
				int global_idx = (result.start_y + y) * data->width + (result.start_x + x);
				int local_idx = y * result.width + x;
				
				task_pixels[local_idx] = data->pixels[global_idx];
				x++;
			}
			y++;
		}
		header.type = MSG_PIXELS;
		header.size = sizeof(t_net_result) + pixels_size;
		send_all(sock, &header, sizeof(t_net_header));
		send_all(sock, &result, sizeof(t_net_result));
		send_all(sock, task_pixels, pixels_size);
		free(task_pixels);
		i++;
	}
}

int recv_full_scene(int server_sock, t_data *data)
{
	t_net_header header;
	t_net_scene_base base;

	recv_all(server_sock, &header, sizeof(t_net_header));
	if (header.type != MSG_SCENE)
		return (-1);
	recv_all(server_sock, &base, sizeof(t_net_scene_base));
	data->cam = base.cam;
	data->view_port = base.view_port;
	data->alight = base.alight;
	data->width = base.width;
	data->height = base.height;
	data->obj_count = base.obj_count;
	data->nodes_used = base.bvh_node_count;
	data->s_per_pixs = base.s_per_pixs;
	data->deph = base.deph;
	data->plane_count = base.plane_count;
	data->use_bvh = base.use_bvh;
	data->step = base.step;
	data->debug = base.debug;
	data->debug_depth = base.debug_depth;
	data->diff_ok = base.diff_ok;
	data->has_checker = base.has_checker;
	data->checker_color = base.checker_color;
	data->light_count = base.light_count;
	t_net_obj *net_objs = malloc(sizeof(t_net_obj) * base.obj_count);
	t_net_obj *net_objs_plane = malloc(sizeof(t_net_obj) * base.plane_count);
	t_net_obj *net_objs_sorted = malloc(sizeof(t_net_obj) * base.obj_count);
	recv_all(server_sock, net_objs, sizeof(t_net_obj) * base.obj_count);
	recv_all(server_sock, net_objs_sorted, sizeof(t_net_obj) * base.obj_count);
	recv_all(server_sock, net_objs_plane, sizeof(t_net_obj) * base.plane_count);
	data->array_obj = malloc(sizeof(t_obj) * base.obj_count);
	data->plane_array = malloc(sizeof(t_obj) * base.plane_count);
	for (int i = 0; i < base.obj_count; i++)
	{
		memcpy(&data->array_obj[i], &net_objs[i], sizeof(t_net_obj));
		data->array_obj[i].has_texture = false;
		data->array_obj[i].has_bump = false;
		if (i < base.obj_count - 1)
			data->array_obj[i].next = &data->array_obj[i + 1];
		else
			data->array_obj[i].next = NULL;
	}
	int i = 0;
	data->sorted_objs = malloc(sizeof(t_obj *) * data->obj_count);
	if (!data->sorted_objs)
		clean_exit(data, 1, "Malloc", 0);
	while (i < data->obj_count)
	{
		data->sorted_objs[i] = &data->array_obj[i];
		i++;
	}
	for (int i = 0; i < base.plane_count; i++)
	{
		memcpy(&data->plane_array[i], &net_objs_plane[i], sizeof(t_net_obj));
		data->plane_array[i].has_texture = false;
		data->plane_array[i].has_bump = false;
		if (i < base.plane_count - 1)
			data->plane_array[i].next = &data->plane_array[i + 1];
		else
			data->plane_array[i].next = NULL;
	}
	free(net_objs);
	free(net_objs_sorted);
	free(net_objs_plane);
	data->bvh_nodes = malloc(sizeof(t_bvh_node) * base.bvh_node_count);
	recv_all(server_sock, data->bvh_nodes, sizeof(t_bvh_node) * base.bvh_node_count);
	data->light = malloc(sizeof(t_light) * base.light_count);
	recv_all(server_sock, data->light, sizeof(t_light) * base.light_count);
	return (0);
}

void	process_and_send_task(t_data *data, int sock, t_net_task task[256], int task_count)
{
	t_thread_info	infos;
	int				expected_tasks;

	expected_tasks = task_count;
	pthread_mutex_lock(&data->finish_count);
	data->finish = 0;
	pthread_mutex_unlock(&data->finish_count);
	int i = 0;
	while (i < task_count)
	{
		infos.data = data;
		infos.start_x = task[i].start_x;
		infos.end_x = task[i].end_x;
		infos.start_y = task[i].start_y;
		infos.end_y = task[i].end_y;
		add_task(data, render, infos);
		i++;
	}
	while (1)
	{
		pthread_mutex_lock(&data->finish_count);
		int finished = data->finish;
		pthread_mutex_unlock(&data->finish_count);
		print_progress(finished, task_count);
		if (finished == expected_tasks)
			break ;
		usleep(100);
	}
	send_task_results(sock, data, task, task_count);
}

void	worker_loop(t_data *data, int sock)
{
	t_net_header	header;
	t_net_task		tasks[256];
	int				task_count;

	task_count = 0;
	while (task_count < 256)
	{
		if (recv_all(sock, &header, sizeof(t_net_header)) < 0)
		{
			printf("Connexion perdue.\n");
			close(sock);
			return ;
		}
		if (header.type == MSG_TASK)
		{
			printf("task n%d recue.\n", task_count);
			if (recv_all(sock, &tasks[task_count], sizeof(t_net_task)) < 0)
			{
				close(sock);
				return ;
			}
			task_count++;
		}
		else if (header.type == MSG_END_TASKS)
			break ;
	}
	process_and_send_task(data, sock, tasks, task_count);
}

void	run_worker(t_data *data, char *master_ip)
{
    int                 sock;
    struct sockaddr_in  serv_addr;
    bool                first_time = true;

    printf("Demarrage du plot. Connexion à %s...\n", master_ip);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        clean_exit(data, 1, "Erreur socket client\n", 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr(master_ip);
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        clean_exit(data, 1, "Connexion au Maitre wu ninjago maitre pas reussite\n", 0);
    printf("Connecte !\n");
    data->pixels = NULL;
    while (1)
    {
        printf("En attente de la prochaine frame...\n");
        if (recv_full_scene(sock, data) < 0)
        {
            printf("Le Maitre s'est deconnecte. Fin du plot.\n");
            break ;
        }
        
        if (!data->pixels)
            data->pixels = malloc(sizeof(mlx_color) * data->width * data->height);
        if (first_time)
        {
            printf("Scene reçue ! Lancement des threads locaux...\n");
            data->thread_running = true;
            init_thread_p(data);
            first_time = false;
        }
        worker_loop(data, sock);
    }
    clean_exit(data, 0, NULL, 0);
}
