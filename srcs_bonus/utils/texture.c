/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 13:47:56 by titan             #+#    #+#             */
/*   Updated: 2026/03/09 17:04:54 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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
		clean_exit(data, 1, "Error: Texture file missing or permission denied\n", 0);
	}
}

t_texture	*check_existing_tex(t_data *data, char *filepath)
{
	t_list	*tmp;
	t_texture	*tex_tmp;

	tex_tmp = NULL;
	tmp = data->textures;
	while (tmp)
	{
		tex_tmp = (t_texture *)tmp->content;
		if (tex_tmp)
		{
			if (!ft_strcmp(filepath, tex_tmp->name))
			{
    			free(filepath);
    			return (tex_tmp);
			}
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	exit_tex(t_data *data, char *filepath, char *file_o, char *mess_error)
{
	if (file_o)
		free(file_o);
	if (filepath)
		free(filepath);
	munmap(data->v_obj->str, data->v_obj->file_size);
	free(data->v_obj->v);
	free(data->v_obj->vn);
	free(data->v_obj->vt);
	clean_exit(data, 1, mess_error, 0);
}

t_texture	*load_texture(t_data *data, char *filepath, char *file_o, int s)
{
	t_texture	*tex;
	t_list		*node;

	check_access(data, filepath, file_o);
	tex = check_existing_tex(data, filepath);
	if (tex)
		return (tex);
	tex = ft_calloc(1, sizeof(t_texture));
	if (!tex)
		exit_tex(data, filepath, file_o, "img load\n");
	node = ft_lstnew(tex);
	if (!node)
		exit_tex(data, filepath, file_o, "Error: Malloc\n");
	ft_lstadd_back(&data->textures, node);
	tex->img = mlx_new_image_from_file(data->mlx, filepath, &tex->width, &tex->height);
	tex->name = ft_strdup(filepath);
	free(filepath);
	if (!tex->img)
		exit_tex(data, NULL, file_o, "img load\n");
	tex->scale = s;
	tex->pixels = malloc(sizeof(mlx_color) * tex->height * tex->width);
	if (!tex->pixels)
		exit_tex(data, NULL, file_o, "img load\n");
	mlx_get_image_region(data->mlx, tex->img, 0, 0, tex->width, tex->height, tex->pixels);
	return (tex);
}

mlx_color	get_texture_color(t_texture *tex, double u, double v)
{
	int	x;
	int	y;
	int	index;

	u = u * tex->scale;
	v = v * tex->scale;
	u = u - floor(u);
	v = v - floor(v);
	x = (int)(u * tex->width);
	y = (int)((1.0 - v) * tex->height);
	if (x >= tex->width)
		x = tex->width - 1;
	if (y >= tex->height)
		y = tex->height - 1;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	index = (y * tex->width) + x;
	return (tex->pixels[index]);
}
