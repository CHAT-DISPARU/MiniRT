/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:01:07 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 11:02:48 by gajanvie         ###   ########.fr       */
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

void	free_texs(t_data *data)
{
	t_texture	*tex;
	t_list		*tmp_t;

	while (data->textures)
	{
		tex = (t_texture *)data->textures->content;
		if (tex->pixels)
			free(tex->pixels);
		if (tex->img)
			mlx_destroy_image(data->mlx, tex->img);
		if (tex->name)
			free(tex->name);
		free(tex);
		tmp_t = data->textures->next;
		free(data->textures);
		data->textures = tmp_t;
	}
	data->mtl_info = NULL;
	data->textures = NULL;
	data->objs = NULL;
}

void	error_arg(void)
{
	ft_putstr_fd("invalid argument:\n./MiniRT <file.rt>\nOptional : ./M", 2);
	ft_putstr_fd("iniRT <file.rt> <resolution> <deph> <sample per pixel>\n", 2);
}

void	re_init(t_data *data)
{
	data->camera_is_set = false;
	data->ambient_is_set = false;
	data->sorted_objs = NULL;
	data->bvh_nodes = NULL;
	data->scene_fd = -1;
	data->scene_line = NULL;
	data->objs = NULL;
	data->light = NULL;
	srand(time(NULL));
}
