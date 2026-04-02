/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 09:24:44 by titan             #+#    #+#             */
/*   Updated: 2026/04/02 14:39:37 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	get_tex_index(t_list *bank, t_texture *target)
{
	int		i = 0;
	t_list	*tmp = bank;

	if (!target)
		return (-1);
	while (tmp)
	{
		if ((t_texture *)tmp->content == target)
			return (i);
		tmp = tmp->next;
		i++;
	}
	return (-1);
}

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

int    send_texture(int socket, t_texture *tex)
{
	t_net_texture    net_tex;
	uint32_t        pixels_size;

	net_tex.width = tex->width;
	net_tex.height = tex->height;
	net_tex.scale = tex->scale;
	net_tex.name_len = tex->name ? ft_strlen(tex->name) : 0;

	if (send_all(socket, &net_tex, sizeof(t_net_texture)) < 0)
		return (-1);
	if (net_tex.name_len > 0)
		if (send_all(socket, tex->name, net_tex.name_len) < 0)
			return (-1);
	pixels_size = tex->width * tex->height * sizeof(mlx_color);
	if (send_all(socket, tex->pixels, pixels_size) < 0)
		return (-1);
	return (0);
}

int	send_all(int socket, const void *buffer, size_t size)
{
	size_t		total_sent = 0;
	ssize_t		n;
	const char	*ptr = (const char *)buffer;

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
	base.a_final = data->a_final;
	base.use_bvh = data->use_bvh;
	base.plane_count = data->plane_count;
	base.light_count = data->light_count;
	int tex_count = ft_lstsize(data->textures);
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
	send_all(client_sock, &tex_count, sizeof(int));
	t_list *tmp = data->textures;
	while (tmp)
	{
		send_texture(client_sock, (t_texture *)tmp->content);
		tmp = tmp->next;
	}
	t_net_obj *net_objs = malloc(sizeof(t_net_obj) * base.obj_count);
	t_net_obj *net_objs_plane = malloc(sizeof(t_net_obj) * base.plane_count);
	t_net_obj *net_objs_sorted = malloc(sizeof(t_net_obj) * base.obj_count);
	for (int i = 0; i < base.obj_count; i++)
	{
		memcpy(&net_objs[i], &data->array_obj[i], sizeof(t_net_obj));
		memcpy(&net_objs_sorted[i], data->sorted_objs[i], sizeof(t_net_obj));
		net_objs[i].tex_index = get_tex_index(data->textures, data->array_obj[i].tex);
		net_objs[i].bump_index = get_tex_index(data->textures, data->array_obj[i].bump);
		net_objs_sorted[i].tex_index = get_tex_index(data->textures, data->sorted_objs[i]->tex);
		net_objs_sorted[i].bump_index = get_tex_index(data->textures, data->sorted_objs[i]->bump);
	}
	for (int i = 0; i < base.plane_count; i++)
	{
		memcpy(&net_objs_plane[i], &data->plane_array[i], sizeof(t_net_obj));
		net_objs_plane[i].tex_index = get_tex_index(data->textures, data->plane_array[i].tex);
		net_objs_plane[i].bump_index = get_tex_index(data->textures, data->plane_array[i].bump);
	}
	send_all(client_sock, net_objs, sizeof(t_net_obj) * base.obj_count);
	send_all(client_sock, net_objs_sorted, sizeof(t_net_obj) * base.obj_count);
	send_all(client_sock, net_objs_plane, sizeof(t_net_obj) * base.plane_count);
	free(net_objs);
	free(net_objs_plane);
	free(net_objs_sorted);
	send_all(client_sock, data->bvh_nodes, sizeof(t_bvh_node) * base.bvh_node_count);
	t_light *light_array = malloc(sizeof(t_light) * base.light_count);
	if (!light_array)
		return (-1);
	t_light *current_light = data->light;
	int j = 0;
	while (current_light && j < base.light_count)
	{
		memcpy(&light_array[j], current_light, sizeof(t_light));
		current_light = current_light->next;
		j++;
	}
	send_all(client_sock, light_array, sizeof(t_light) * base.light_count);
	free(light_array);
	return (0);
}

int	send_scene_low(int client_sock, t_data *data)
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
	base.s_per_pixs = data->s_per_pixs;
	base.deph = data->deph;
	base.a_final = data->a_final;
	base.use_bvh = data->use_bvh;
	header.type = MSG_SCENE_LOW;
	header.size = sizeof(t_net_scene_base);
	send_all(client_sock, &header, sizeof(t_net_header));
	send_all(client_sock, &base, sizeof(t_net_scene_base));
	return (0);
}

int	send_restart(int client_sock)
{
	t_net_header		header;

	header.type = MSG_RESTART;
	header.size = sizeof(t_net_header);
	send_all(client_sock, &header, sizeof(t_net_header));
	return (0);
}
