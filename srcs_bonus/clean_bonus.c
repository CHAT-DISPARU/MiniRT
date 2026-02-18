/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:10:27 by titan             #+#    #+#             */
/*   Updated: 2026/02/18 11:19:18 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	print_m_and_gnl(t_data *data, char *mess_eror, int exit_code, int i)
{
	if (data->scene_line)
	{
		free(data->scene_line);
		data->scene_line = NULL;
	}
	if (data->scene_fd != -1)
	{
		get_next_line(data->scene_fd, 1);
		close(data->scene_fd);
	}
	if (exit_code != EXIT_SUCCESS)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd(mess_eror, 2);
	}
	if (i > 0)
	{
		ft_putstr_fd("line : ", 2);
		ft_putstr_fd(ft_itoa(i), 2);
		ft_putstr_fd("\n", 2);
	}
}

void	clean(t_data *data)
{
	t_obj		*tmp;
	t_obj		*current;
	t_light		*tmp2;
	t_texture	*tex;
	t_list		*tmp_t;

	printf("\n");
	current = data->objs;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	while (data->textures)
	{
		tex = (t_texture *)data->textures->content;
		if (tex->pixels)
			free(tex->pixels);
		if (tex->img)
			mlx_destroy_image(data->mlx, tex->img);
		free(tex);
		tmp_t = data->textures->next;
		free(data->textures);
		data->textures = tmp_t;
	}
	data->textures = NULL;
	data->objs = NULL;
	while (data->light)
	{
		tmp2 = data->light->next;
		free(data->light);
		data->light = tmp2;
	}
	if (data->array_obj)
		free(data->array_obj);
	if (data->bvh_nodes)
		free(data->bvh_nodes);
	if (data->sorted_objs)
		free(data->sorted_objs);
	if (data->plane_array)
		free(data->plane_array);
	if (data->obj_aabbs)
		free(data->obj_aabbs);
	if (data->scene_line)
	{
		free(data->scene_line);
		data->scene_line = NULL;
	}
	if (data->scene_fd != -1)
	{
		get_next_line(data->scene_fd, 1);
		close(data->scene_fd);
	}
}

void	clean_exit(t_data *data, int exit_code, char *mess_eror, int i)
{
	t_obj		*tmp;
	t_obj		*current;
	t_light		*tmp2;
	t_texture	*tex;
	t_list		*tmp_t;

	printf("\n");
	stop_threads(data);
	print_m_and_gnl(data, mess_eror, exit_code, i);
	current = data->objs;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	while (data->textures)
	{
		tex = (t_texture *)data->textures->content;
		if (tex->pixels)
			free(tex->pixels);
		if (tex->img)
			mlx_destroy_image(data->mlx, tex->img);
		free(tex);
		tmp_t = data->textures->next;
		free(data->textures);
		data->textures = tmp_t;
	}
	data->textures = NULL;
	data->objs = NULL;
	while (data->light)
	{
		tmp2 = data->light->next;
		free(data->light);
		data->light = tmp2;
	}
	if (data->array_obj)
		free(data->array_obj);
	if (data->bvh_nodes)
		free(data->bvh_nodes);
	if (data->sorted_objs)
		free(data->sorted_objs);
	if (data->plane_array)
		free(data->plane_array);
	if (data->obj_aabbs)
		free(data->obj_aabbs);
	if (!data)
		exit(exit_code);
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_context(data->mlx);
	if (data->pixels)
		free(data->pixels);
	free(data);
	exit(exit_code);
}
