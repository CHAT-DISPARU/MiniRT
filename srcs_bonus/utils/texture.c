/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 13:47:56 by titan             #+#    #+#             */
/*   Updated: 2026/02/13 16:14:40 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

#include <stdio.h>
#include <stdlib.h>

t_texture	*load_texture(t_data *data, char *filepath)
{
	t_texture	*tex;

	tex = ft_calloc(1, sizeof(t_texture));
	if (!tex)
		clean_exit(data, 1, "img load\n", 0);
	tex->img = mlx_new_image_from_file(data->mlx, filepath, &tex->width, &tex->height);
	free(filepath);
	if (!tex->img)
		clean_exit(data, 1, "img load\n", 0);
	tex->pixels = malloc(sizeof(mlx_color) * tex->height * tex->width);
	if (!tex->pixels)
		clean_exit(data, 1, "malloc\n", 0);
	mlx_get_image_region(data->mlx, tex->img, 0, 0, tex->width, tex->height, tex->pixels);
	return (tex);
}

#include <math.h>

mlx_color	get_texture_color(t_texture *tex, double u, double v)
{
	int	x;
	int	y;
	int	index;
	double	scale = 1;

	u = u * scale;
	v = v * scale;
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
