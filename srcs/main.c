/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:41:49 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/19 16:05:01 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

void	clean_exit(t_data *data, int exit_code)
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

void	key_up(int key, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (key >= 0 && key < 512)
		data->key_table[key] = 0;
}

void	key_down(int key, void *param)
{
	t_data	*data;

	printf("%d\n", key);
	data = (t_data *)param;
	if (key == 41)
		mlx_loop_end(data->mlx);
	if (key >= 0 && key < 512)
		data->key_table[key] = 1;
}

void	window_hook(int event, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (event == 0)
		mlx_loop_end(data->mlx);
	else if (event == 1)
	{
		if (resize_win(data) != 0)
			mlx_loop_end(data->mlx);
	}
}

/*
	D direction O origine t inconue

	Ray(t) = o + t * D;


	tous les points f d'une sphere

	

	||f - c|| = r²; longeur vecteur au carre est strictement egual au produit scalire de lui meme
	(f - c) * (f - c) = r²
	((O + t * D) - C) * ((O + t * D) - C) = r²
	(OC + TD) * (OC + TD) = r²

	(a + b)² = a² + 2ab + b²

	(OC * OC) + 2(OC * TD) + TD² = r²
	(OC * OC) + 2t(OC * D) + t²(D * D) = r²
	(OC * OC) + 2t(OC * D) + t²(D * D) - r² = 0
*/

bool	hit_sphere(t_vec3 sphere_center, double radius, t_ray r)
{
	t_vec3	ocenter;
	double	a;
	double	b;
	double	c;
	double	dis;

	ocenter = vec_sub(r.origin, sphere_center);
	a = vec_dot(r.dir, r.dir);
	b = 2.0 * vec_dot(ocenter, r.dir);
	c = vec_dot(ocenter, ocenter) - (radius * radius);
	dis = (b * b) - (4 * a * c);
	if (dis < 0)
		return (false);
	return (true);
}

void	render(t_data *data)
{
	int			y;
	int			x;
	double		u;
	double		v;
	t_ray		ray;
	int			idx;
	double		aspect_ratio = (double)data->width / (double)data->height;
	double		viewport_height = 2;
	double		viewport_width = aspect_ratio * viewport_height;
	double		dis_cam_viewp = 1;

	y = 0;
	data->test_sphere.center.x = -1;
	data->test_sphere.center.y = 0;
	data->test_sphere.center.z = -10;
	data->test_sphere.radius = 2;
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFFFFFFFF});
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			u = (double)x / (data->width - 1);
			v = (double)y / (data->height - 1);
			ray.origin = data->camera_origin;
			ray.dir.x = (u * viewport_width) - (viewport_width / 2);
			ray.dir.y = (viewport_height / 2) - (v * viewport_height);
			ray.dir.z = -dis_cam_viewp;
			ray.dir = vec_normalize(ray.dir);
			idx = y * data->width + x;
			if (hit_sphere(data->test_sphere.center, data->test_sphere.radius, ray))
				data->pixels[idx].rgba = 0x847E08FF;
			else
				data->pixels[idx].rgba = 0x000000FF;
			x++;
		}
		y++;
	}
	mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
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
	render(data);
	return (0);
}

void	update(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (data->key_table[68] && !data->old_key_table[68])
	{
		data->is_full = !data->is_full;
		mlx_set_window_fullscreen(data->mlx, data->win, data->is_full);
	}
	if (data->key_table[82])
	{
		data->camera_origin.y += 0.5;
		render(data);
	}
	if (data->key_table[81])
	{
		data->camera_origin.y -= 0.5;
		render(data);
	}
	if (data->key_table[80])
	{
		data->camera_origin.x -= 0.5;
		render(data);
	}
	if (data->key_table[79])
	{
		data->camera_origin.x += 0.5;
		render(data);
	}
	ft_memcpy(data->old_key_table, data->key_table, sizeof(data->key_table));
}

int	main(void)
{
	t_data					*data;
	mlx_window_create_info	info;

	ft_memset(&info, 0, sizeof(mlx_window_create_info));
	data = ft_calloc(sizeof(t_data), 1);
	if (!data)
	{
		perror("Malloc fail");
		return (1);
	}
	data->width = WIDTH;
	data->height = HEIGHT;
	data->is_full = false;
	srand(time(NULL));
	ft_memset(data->key_table, 0, sizeof(data->key_table));
	ft_bzero(&data->camera_origin, sizeof(data->camera_origin));
	data->mlx = mlx_init();
	if (!data->mlx)
		clean_exit(data, 1);
	info.title = "MiniRT";
	info.width = WIDTH;
	info.height = HEIGHT;
	info.is_resizable = true;
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		clean_exit(data, 1);
	if (resize_win(data) != 0)
		clean_exit(data, 1);
	mlx_set_fps_goal(data->mlx, 60);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_down, data);
	mlx_on_event(data->mlx, data->win, MLX_KEYUP, key_up, data);
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook, data);
	mlx_add_loop_hook(data->mlx, update, data);
	mlx_loop(data->mlx);
	clean_exit(data, 0);
	return (0);
}
