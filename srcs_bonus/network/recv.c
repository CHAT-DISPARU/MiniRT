/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 09:31:00 by CHAT-DISPAR       #+#    #+#             */
/*   Updated: 2026/04/01 10:44:29 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	recv_texture(int socket, t_texture *tex)
{
	t_net_texture	net_tex;
	uint32_t		pixels_size;

	if (recv_all(socket, &net_tex, sizeof(t_net_texture)) < 0)
		return (-1);
	tex->width = net_tex.width;
	tex->height = net_tex.height;
	tex->scale = net_tex.scale;
	tex->img = NULL;
	tex->name = NULL;
	pixels_size = tex->width * tex->height * sizeof(mlx_color);
	tex->pixels = malloc(pixels_size);
	if (!tex->pixels)
		return (-1);
	if (recv_all(socket, tex->pixels, pixels_size) < 0)
		return (-1);
	return (0);
}

int	recv_full_scene(int server_sock, t_data *data)
{
	t_net_header		header;
	t_net_scene_base	base;

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
	data->a_final = base.a_final;
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
		if (data->array_obj[i].has_texture == true)
		{
			data->array_obj[i].tex = malloc(sizeof(t_texture));
			recv_texture(server_sock, data->array_obj[i].tex); 
		}
		else
			data->array_obj[i].tex = NULL;
		if (data->array_obj[i].has_bump == true)
		{
			data->array_obj[i].bump = malloc(sizeof(t_texture));
			recv_texture(server_sock, data->array_obj[i].bump);
		}
		else
			data->array_obj[i].bump = NULL;

		if (i < base.obj_count - 1)
			data->array_obj[i].next = &data->array_obj[i + 1];
		else
			data->array_obj[i].next = NULL;
	}
	int i = 0;
	data->sorted_objs = malloc(sizeof(t_obj *) * data->obj_count);
	t_obj *sor_o_test = malloc(sizeof(t_obj) * data->obj_count);
	if (!data->sorted_objs)
		clean_exit(data, 1, "Malloc", 0);
	while (i < data->obj_count)
	{
		memcpy(&sor_o_test[i], &net_objs_sorted[i], sizeof(t_net_obj));
		data->sorted_objs[i] = &sor_o_test[i];
		i++;
	}
	for (int i = 0; i < base.plane_count; i++)
	{
		memcpy(&data->plane_array[i], &net_objs_plane[i], sizeof(t_net_obj));
		if (data->plane_array[i].has_texture == true)
		{
			data->plane_array[i].tex = malloc(sizeof(t_texture));
			recv_texture(server_sock, data->plane_array[i].tex); 
		}
		else
			data->plane_array[i].tex = NULL;
		if (data->plane_array[i].has_bump == true)
		{
			data->plane_array[i].bump = malloc(sizeof(t_texture));
			recv_texture(server_sock, data->plane_array[i].bump);
		}
		else
			data->plane_array[i].bump = NULL;

		if (i < base.plane_count - 1)
			data->plane_array[i].next = &data->plane_array[i + 1];
		else
			data->plane_array[i].next = NULL;
	}
	free(net_objs);
	free(net_objs_sorted);
	free(net_objs_plane);
	data->bvh_nodes = ft_calloc(sizeof(t_bvh_node), base.bvh_node_count);
	recv_all(server_sock, data->bvh_nodes, sizeof(t_bvh_node) * base.bvh_node_count);
	data->light = malloc(sizeof(t_light) * base.light_count);
	recv_all(server_sock, data->light, sizeof(t_light) * base.light_count);
	for (int i = 0; i < base.light_count; i++)
	{
		if (i < base.light_count - 1)
			data->light[i].next = &data->light[i + 1];
		else
			data->light[i].next = NULL;
	}
	return (0);
}

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

void	recv_task_result(int sock, t_data *data, uint32_t header_size)
{
	t_net_result	result;
	mlx_color		*incoming_pixels;
	uint32_t		pixels_size;

	recv_all(sock, &result, sizeof(t_net_result));
	pixels_size = header_size - sizeof(t_net_result);
	incoming_pixels = malloc(pixels_size);
	recv_all(sock, incoming_pixels, pixels_size);
	int	i = 0;
	for (int y = 0; y < result.height; y++)
	{
		for (int x = 0; x < result.width; x++)
		{
			int	global_y = result.start_y + y;
			int	global_x = result.start_x + x;
			int	global_index = global_y * data->width + global_x;
			data->pixels[global_index] = incoming_pixels[i++];
		}
	}
	free(incoming_pixels);
}
