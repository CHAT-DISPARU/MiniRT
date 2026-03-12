/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:52:20 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 11:00:12 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	relaunch(bool *movded, t_data *data)
{
	if (data->key_table[229] && !data->old_key_table[229])
	{
		clean(data);
		re_init(data);
		read_rt(data);
		convert_list_to_arrays(data);
		calcul_ambient(data);
		*movded = true;
	}
}

void	check_access(t_data *data, char *filepath, char *file_o)
{
	if (access(filepath, F_OK | R_OK) == -1)
	{
		if (file_o)
			free(file_o);
		if (filepath)
			printf("%s\n", filepath);
		free(filepath);
		munmap(data->v_obj->str, data->v_obj->file_size);
		free(data->v_obj->v);
		free(data->v_obj->vn);
		free(data->v_obj->vt);
		clean_exit(data, 1,
			"Error: Texture file missing or permission denied\n", 0);
	}
}

double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + (tv.tv_usec / 1000000.0));
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

void	set_indexs(int *indexs)
{
	int	i;
	int	tmp;
	int	j;

	i = 0;
	while (i < THREADS_COUNT * NB_TASK_R)
	{
		indexs[i] = i;
		i++;
	}
	i = THREADS_COUNT * NB_TASK_R - 1;
	while (i > 0)
	{
		j = rand() % (i + 1);
		tmp = indexs[i];
		indexs[i] = indexs[j];
		indexs[j] = tmp;
		i--;
	}
}
