/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_part2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:55:32 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:56:02 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	resize_win(t_data *data)
{
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
		build_bvh(data);
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
