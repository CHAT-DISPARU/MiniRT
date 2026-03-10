/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:17:57 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 13:49:01 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	ft_facesize(t_face_c *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	ft_faceadd_back(t_face_c **lst, t_face_c *new)
{
	t_face_c	*current;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	ft_lightadd_back(t_light **lst, t_light *new)
{
	t_light	*current;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	ft_objadd_back(t_obj **lst, t_obj *new)
{
	t_obj	*current;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	init_t_calc_f(t_calc_f *functions)
{
	functions[CALC_SQ] = hit_square;
	functions[CALC_CY] = hit_cylinder;
	functions[CALC_PL] = hit_plane;
	functions[CALC_SP] = hit_sphere;
	functions[CALC_TR] = hit_triangle;
	functions[CALC_HY] = hit_hyperboloid;
	functions[CALC_CO] = hit_cone;
}

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

t_aabb	get_aabb_by_type(t_obj *obj)
{
	if (obj->type == CALC_SP)
		return (aabb_sphere(obj));
	else if (obj->type == CALC_CY)
		return (aabb_cylinder(obj));
	else if (obj->type == CALC_CO)
		return (aabb_cone(obj));
	else if (obj->type == CALC_SQ)
		return (aabb_square(obj));
	else if (obj->type == CALC_HY)
		return (aabb_hyperboloid(obj));
	else
		return (aabb_triangle(obj));
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

double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + (tv.tv_usec / 1000000.0));
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
