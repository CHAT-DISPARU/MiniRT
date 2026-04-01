/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:18:46 by gajanvie          #+#    #+#             */
/*   Updated: 2026/04/01 10:46:00 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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
	int					sock;
	struct sockaddr_in	serv_addr;
	bool				first_time = true;

	printf("Demarrage du gros plot de chantier. Connexion à %s...\n", master_ip);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		clean_exit(data, 1, "Erreur socket client\n", 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);
	serv_addr.sin_addr.s_addr = inet_addr(master_ip);
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		clean_exit(data, 1, "Connexion au Maitre wu ninjago maitre pas reussite\n", 0);
	printf("Connecte Prime!!\n");
	data->pixels = NULL;
	while (1)
	{
		printf("En attente de la prochaine frame...\n");
		if (recv_full_scene(sock, data) < 0)
		{
			printf("Le Maitre ninjago sensei wu s'est deconnecte. Fin du gros plot de chantier.\n");
			break ;
		}
		if (!data->pixels)
			data->pixels = malloc(sizeof(mlx_color) * data->width * data->height);
		if (first_time)
		{
			printf("Scene recue !!!!!! Lancement des threads locaux...\n");
			data->thread_running = true;
			init_thread_p(data);
			first_time = false;
		}
		worker_loop(data, sock);
		usleep(100);
	}
	clean_exit(data, 0, NULL, 0);
}
