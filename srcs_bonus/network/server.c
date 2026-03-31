/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:18:36 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/31 17:54:02 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	recv_all(int socket, void *buffer, size_t size)
{
	size_t	total_recv = 0;
	ssize_t	n;
	char	*ptr = (char *)buffer;

	while (total_recv < size)
	{
		n = recv(socket, ptr + total_recv, size - total_recv, 0);
		if (n <= 0)
			return (-1);
		total_recv += n;
	}
	return (0);
}

void	recv_task_result(int sock, t_data *data, uint32_t payload_size)
{
	t_net_result    result;
	mlx_color       *incoming_pixels;
	uint32_t        pixels_size;

	recv_all(sock, &result, sizeof(t_net_result));
	pixels_size = payload_size - sizeof(t_net_result);
	incoming_pixels = malloc(pixels_size);
	recv_all(sock, incoming_pixels, pixels_size);
	int i = 0;
	for (int y = 0; y < result.height; y++)
	{
		for (int x = 0; x < result.width; x++)
		{
			int global_y = result.start_y + y;
			int global_x = result.start_x + x;
			int global_index = global_y * data->width + global_x;
			data->pixels[global_index] = incoming_pixels[i++];
		}
	}
	free(incoming_pixels);
}

int	send_all(int socket, const void *buffer, size_t size)
{
	size_t total_sent = 0;
	ssize_t n;
	const char *ptr = (const char *)buffer;

	while (total_sent < size)
	{
		n = send(socket, ptr + total_sent, size - total_sent, 0);
		if (n <= 0)
			return (-1);
		total_sent += n;
	}
	return (0);
}

int	send_full_scene(int client_sock, t_data *data)
{
	t_net_header		header;
	t_net_scene_base	base;

	base.step = data->step;
	base.debug = data->debug;
	base.debug_depth = data->debug_depth;
	base.diff_ok = data->diff_ok;
	base.has_checker = data->has_checker;
	base.checker_color = data->checker_color;
	base.cam = data->cam;
	base.view_port = data->view_port;
	base.alight = data->alight;
	base.width = data->width;
	base.height = data->height;
	base.obj_count = data->obj_count;
	base.bvh_node_count = data->nodes_used;
	base.s_per_pixs = data->s_per_pixs;
	base.deph = data->deph;
	base.use_bvh = data->use_bvh;
	base.plane_count = data->plane_count;
	base.light_count = data->light_count;
	uint32_t payload_size = sizeof(t_net_scene_base) 
		+ (base.obj_count * sizeof(t_net_obj))
		+ (base.obj_count * sizeof(t_net_obj))
		+ (base.bvh_node_count * sizeof(t_bvh_node))
		+ (base.light_count * sizeof(t_light))
		+ (base.plane_count * sizeof(t_net_obj));
	header.type = MSG_SCENE;
	header.size = payload_size;
	send_all(client_sock, &header, sizeof(t_net_header));
	send_all(client_sock, &base, sizeof(t_net_scene_base));
	t_net_obj *net_objs = malloc(sizeof(t_net_obj) * base.obj_count);
	t_net_obj *net_objs_plane = malloc(sizeof(t_net_obj) * base.plane_count);
	t_net_obj *net_objs_sorted = malloc(sizeof(t_net_obj) * base.obj_count);
	for (int i = 0; i < base.obj_count; i++)
	{
		memcpy(&net_objs[i], &data->array_obj[i], sizeof(t_net_obj));
		memcpy(&net_objs_sorted[i], data->sorted_objs[i], sizeof(t_net_obj));
	}
	for (int i = 0; i < base.plane_count; i++)
		memcpy(&net_objs_plane[i], &data->plane_array[i], sizeof(t_net_obj));
	send_all(client_sock, net_objs, sizeof(t_net_obj) * base.obj_count);
	send_all(client_sock, net_objs_sorted, sizeof(t_net_obj) * base.obj_count);
	send_all(client_sock, net_objs_plane, sizeof(t_net_obj) * base.plane_count);
	free(net_objs);
	free(net_objs_plane);
	free(net_objs_sorted);
	send_all(client_sock, data->bvh_nodes, sizeof(t_bvh_node) * base.bvh_node_count);
	send_all(client_sock, data->light, sizeof(t_light) * base.light_count);
	return (0);
}

void	init_server(t_data *data)
{
    struct sockaddr_in  server_addr;
    int                 opt = 1;

    data->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (data->server_socket < 0)
        clean_exit(data, 1, "Erreur creation socket\n", 0);
    setsockopt(data->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    fcntl(data->server_socket, F_SETFL, O_NONBLOCK);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    if (bind(data->server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        clean_exit(data, 1, "Erreur bind socket\n", 0);
    if (listen(data->server_socket, 10) < 0)
        clean_exit(data, 1, "Erreur listen socket\n", 0);
    data->client_count = 0;
}

void	check_new_clients(t_data *data)
{
    int                 new_sock;
    struct sockaddr_in  client_addr;
    socklen_t           addr_len = sizeof(client_addr);

    while (1)
    {
        new_sock = accept(data->server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (new_sock < 0)
            break ;

        if (data->client_count < 10)
        {
            data->client_sockets[data->client_count] = new_sock;
            data->client_count++;
            printf("\nnouveau plot ! Total : %d\n", data->client_count);
        }
        else
        {
            printf("\nTrop de plots.\n");
            close(new_sock);
        }
    }
}
