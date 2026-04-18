/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:52:20 by gajanvie          #+#    #+#             */
/*   Updated: 2026/04/16 15:34:53 by gajanvie         ###   ########.fr       */
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
		int i;
		i = 0;
		while (i < data->client_count) 
		{
			if (data->client_sockets[i] != -1 && send_restart(data->client_sockets[i]) == -1)
				ft_putstr_fd("error scene envoie\n", 2);
			i++;
		}
		i = 0;
		while (i < data->client_count) 
		{
			if (data->client_sockets[i] != -1 && send_full_scene(data->client_sockets[i], data) == -1)
				ft_putstr_fd("error scene envoie\n", 2);
			i++;
		}
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

void	set_indexs(int *indexs, int indexs_size)
{
	int	i;
	int	tmp;
	int	j;

	i = 0;
	while (i < indexs_size)
	{
		indexs[i] = i;
		i++;
	}
	i = indexs_size - 1;
	while (i > 0)
	{
		j = rand() % (i + 1);
		tmp = indexs[i];
		indexs[i] = indexs[j];
		indexs[j] = tmp;
		i--;
	}
}
