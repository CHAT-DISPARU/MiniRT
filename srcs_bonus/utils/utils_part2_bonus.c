/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_part2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: CHAT-DISPARU <CHAT-DISPARU@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:55:32 by gajanvie          #+#    #+#             */
/*   Updated: 2026/04/18 16:49:10 by CHAT-DISPAR      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	resize_win(t_data *data)
{
	if (data->is_rendering)
		return (0);
	if (data->img)
	{
		mlx_destroy_image(data->mlx, data->img);
		data->img = NULL;
	}
	if (data->pixels)
	{
		free(data->pixels);
		data->pixels = NULL;
	}
	mlx_get_window_size(data->mlx, data->win, &data->width, &data->height);
	data->img = mlx_new_image(data->mlx, data->width, data->height);
	data->pixels = calloc(data->width * data->height, sizeof(mlx_color));
	if (!data->img || !data->pixels)
		return (1);
	thread_calls(data);
	return (0);
}

void	set_arrays2(t_data *data, t_obj *curr, int *i_obj)
{
	data->array_obj[*i_obj] = *curr;
	data->array_obj[*i_obj].next = NULL;
	data->obj_aabbs[*i_obj] = get_aabb_by_type(&data->array_obj[*i_obj]);
	*i_obj += 1;
}

void	set_arrays(t_data *data)
{
	t_obj	*curr;
	int		i_obj;
	int		i_plane;
	t_obj	*tmp;

	i_obj = 0;
	i_plane = 0;
	curr = data->objs;
	while (curr)
	{
		if (curr->type == CALC_PL)
		{
			data->plane_array[i_plane] = *curr;
			data->plane_array[i_plane].next = NULL;
			i_plane++;
		}
		else
			set_arrays2(data, curr, &i_obj);
		tmp = curr;
		curr = curr->next;
		free(tmp);
	}
	data->objs = NULL;
}

void	convert_list_to_arrays(t_data *data)
{
	t_obj	*curr;

	curr = data->objs;
	data->obj_count = 0;
	data->plane_count = 0;
	while (curr)
	{
		if (curr->type == CALC_PL)
			data->plane_count++;
		else
			data->obj_count++;
		curr = curr->next;
	}
	if (data->obj_count > 0)
	{
		data->array_obj = malloc(sizeof(t_obj) * data->obj_count);
		data->obj_aabbs = malloc(sizeof(t_aabb) * data->obj_count);
		if (!data->array_obj || !data->obj_aabbs)
			clean_exit(data, 1, "Malloc failed", 0);
	}
	if (data->plane_count > 0)
		data->plane_array = malloc(sizeof(t_obj) * data->plane_count);
	set_arrays(data);
	if (data->obj_count > 0)
	{
		build_bvh(data);
		init_emissive_lights(data);
	}
}

void	print_progress(int current_line, int total_lines)
{
	int	percent;
	int	bar_width;
	int	pos;
	int	i;

	bar_width = 40;
	percent = (current_line * 100) / total_lines;
	if (percent > 100)
		percent = 100;
	pos = (percent * bar_width) / 100;
	printf("\r\033[0;32mRender: [");
	i = 0;
	while (i < bar_width)
	{
		if (i < pos)
			printf("=");
		else if (i == pos)
			printf(">");
		else
			printf(" ");
		i++;
	}
	printf("] %d%%\033[0m", percent);
	fflush(stdout);
}

void	init_emissive_lights(t_data *data)
{
	int	i;
	int	j;

	data->emissive_count = 0;
	i = 0;
	while (i < data->obj_count)
	{
		if (data->sorted_objs[i]->emission_ratio > 0.0)
			data->emissive_count++;
		i++;
	}
	if (data->emissive_count > 0)
	{
		data->emissive_ids = malloc(sizeof(int) * data->emissive_count);
		if (!data->emissive_ids)
			clean_exit(data, 1, "Malloc failed emissive_ids", 0);
	}
	else
		data->emissive_ids = NULL;
	i = 0;
	j = 0;
	while (i < data->obj_count)
	{
		data->sorted_objs[i]->id = i;
		data->obj_aabbs[i] = get_aabb_by_type(data->sorted_objs[i]); 
		if (data->sorted_objs[i]->emission_ratio > 0.0)
		{
			data->emissive_ids[j] = i;
			j++;
		}
		i++;
	}
}
