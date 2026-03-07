/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 13:47:56 by titan             #+#    #+#             */
/*   Updated: 2026/03/07 17:10:21 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_texture	*load_texture(t_data *data, char *filepath, char *file_o, int s)
{
	t_texture	*tex;
	t_list		*tmp;
	t_list		*node;
	t_texture	*tex_tmp;

	if (access(filepath, F_OK | R_OK) == -1)
	{
		if (file_o)
			free(file_o);
		if (filepath)
			printf("%s\n", filepath);
		free(filepath);
		clean_exit(data, 1, "Error: Texture file missing or permission denied\n", 0);
	}
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
	tex = ft_calloc(1, sizeof(t_texture));
	if (!tex)
	{
		if (file_o)
			free(file_o);
		free(filepath);
		clean_exit(data, 1, "img load\n", 0);
	}
	node = ft_lstnew(tex);
	if (!node)
	{
		if (file_o)
			free(file_o);
		free(tex);
		free(filepath);
		clean_exit(data, 1, "Error: Malloc\n", 0);
	}
	ft_lstadd_back(&data->textures, node);
	tex->img = mlx_new_image_from_file(data->mlx, filepath, &tex->width, &tex->height);
	tex->name = ft_strdup(filepath);
	free(filepath);
	if (!tex->img)
	{
		if (file_o)
			free(file_o);
		clean_exit(data, 1, "img load\n", 0);
	}
	tex->scale = s;
	tex->pixels = malloc(sizeof(mlx_color) * tex->height * tex->width);
	if (!tex->pixels)
	{
		if (file_o)
			free(file_o);
		clean_exit(data, 1, "malloc\n", 0);
	}
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
