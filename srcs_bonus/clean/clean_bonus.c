/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:10:27 by titan             #+#    #+#             */
/*   Updated: 2026/03/11 11:03:11 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	free_element(t_data *data)
{
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
}

void	free_mtl_light(t_data *data)
{
	t_mtl_info	*t;
	t_light		*tmp2;

	while (data->mtl_info)
	{
		if (data->mtl_info->idx)
			free(data->mtl_info->idx);
		t = data->mtl_info->next;
		free(data->mtl_info);
		data->mtl_info = t;
	}
	while (data->light)
	{
		tmp2 = data->light->next;
		free(data->light);
		data->light = tmp2;
	}
}

void	clean(t_data *data)
{
	t_obj		*tmp;
	t_obj		*current;

	printf("\n");
	current = data->objs;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	free_texs(data);
	free_mtl_light(data);
	free_element(data);
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

void	free_data_struct(t_data *data, int exit_code)
{
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

void	clean_exit(t_data *data, int exit_code, char *mess_eror, int i)
{
	t_obj		*tmp;
	t_obj		*current;

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
	free_texs(data);
	free_mtl_light(data);
	free_element(data);
	free_data_struct(data, exit_code);
}
