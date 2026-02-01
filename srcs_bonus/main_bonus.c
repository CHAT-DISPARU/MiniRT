/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:41:49 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/01 14:12:31 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	init_data(t_data *data, mlx_window_create_info info, char *filename)
{
	if (pthread_mutex_init(&data->pixel_mutex, NULL) != 0)
		clean_exit(data, 1, "mutex init\n", 0);
	data->camera_is_set = false;
	data->ambient_is_set = false;
	data->width = WIDTH;
	data->height = HEIGHT;
	data->is_full = false;
	data->scene_fd = -1;
	data->scene_line = NULL;
	data->s_per_pixs = 1;
	data->objs = NULL;
	srand(time(NULL));
	ft_memset(data->key_table, 0, sizeof(data->key_table));
	data->cam.up_guide = (t_vec3){0, 1, 0};
	data->speed = 0.5;
	data->rot_speed = 0.05;
	read_rt(data, filename);
	calcul_ambient(data);
	data->mlx = mlx_init();
	if (!data->mlx)
		clean_exit(data, 1, "Error init MLX\n", 0);
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		clean_exit(data, 1, "Error Malloc MLX\n", 0);
	if (resize_win(data) != 0)
		clean_exit(data, 1, "Error Malloc MLX\n", 0);
}

void	set_info(mlx_window_create_info	*info)
{
	info->title = "MiniRT";
	info->width = WIDTH;
	info->height = HEIGHT;
	info->is_resizable = true;
}

int	main(int ac, char **av)
{
	t_data					*data;
	mlx_window_create_info	info;

	if (ac != 2)
	{
		ft_putstr_fd("invalid argument:\n./MiniRT <file.rt>\n", 2);
		return (1);
	}
	ft_memset(&info, 0, sizeof(mlx_window_create_info));
	data = ft_calloc(sizeof(t_data), 1);
	if (!data)
	{
		perror("Malloc fail");
		return (1);
	}
	set_info(&info);
	init_data(data, info, av[1]);
	mlx_set_fps_goal(data->mlx, 60);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_down, data);
	mlx_on_event(data->mlx, data->win, MLX_KEYUP, key_up, data);
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook, data);
	mlx_add_loop_hook(data->mlx, update, data);
	mlx_loop(data->mlx);
	clean_exit(data, 0, NULL, 0);
	return (0);
}
