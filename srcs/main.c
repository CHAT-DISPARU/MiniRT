/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:41:49 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/18 14:05:10 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

unsigned int	get_random_color(void)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
	return (r << 24 | g << 16 | b << 8 | 0xFF);
}

unsigned int	lerp_color(unsigned int c1, unsigned int c2, float t)
{
	t_lerp_color	lerp;

	lerp.r1 = (c1 >> 24) & 0xFF;
	lerp.g1 = (c1 >> 16) & 0xFF;
	lerp.b1 = (c1 >> 8) & 0xFF;
	lerp.r2 = (c2 >> 24) & 0xFF;
	lerp.g2 = (c2 >> 16) & 0xFF;
	lerp.b2 = (c2 >> 8) & 0xFF;
	lerp.r = (unsigned char)(lerp.r1 + t * (lerp.r2 - lerp.r1));
	lerp.g = (unsigned char)(lerp.g1 + t * (lerp.g2 - lerp.g1));
	lerp.b = (unsigned char)(lerp.b1 + t * (lerp.b2 - lerp.b1));
	return ((lerp.r << 24) | (lerp.g << 16) | (lerp.b << 8) | 0xFF);
}

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
		data->key_table[key] = 1;
}

void	key_down(int key, void *param)
{
	t_data	*data;

	printf("%d\n", key);
	data = (t_data *)param;
	if (key == 41)
		mlx_loop_end(data->mlx);
	if (key == 6)
	{
		data->color_start = get_random_color();
		data->color_end = get_random_color();
		render(data);
	}
	if (key == 68)
	{
		data->is_full = !data->is_full;
		mlx_set_window_fullscreen(data->mlx, data->win, data->is_full);
	}
	if (key >= 0 && key < 512)
		data->key_table[key] = 0;
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

void	render(t_data *data)
{
	int				y;
	int				x;
	int				idx;
	unsigned int	current_color;
	double			ratio;

	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFFFFFFFF});
	if (!data->win || !data->img || !data->pixels)
		return ;
	y = 0;
	while (y < data->height)
	{
		ratio = (double)y / (double)data->height;
		current_color = lerp_color(data->color_start, data->color_end, ratio);
		x = 0;
		while (x < data->width)
		{
			idx = y * data->width + x;
			data->pixels[idx].rgba = current_color;
			x++;
		}
		y++;
	}
	mlx_set_image_region(data->mlx, data->img, 0, 0,
		data->width, data->height, data->pixels);
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
	data->color_start = 0x000000FF;
	data->color_end = 0xFFFFFFFF;
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
	mlx_loop(data->mlx);
	clean_exit(data, 0);
	return (0);
}
