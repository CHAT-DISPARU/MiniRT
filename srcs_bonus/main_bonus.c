/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:41:49 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/03 13:34:40 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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

void	init_data(t_data *data, mlx_window_create_info info, char **av, int ac)
{
	data->camera_is_set = false;
	if (ac >= 3)
		data->step = ft_atoi(av[2]);
	else if (ac < 3 || data->step <= 0)
		data->step = 1;
	if (ac >= 4)
		data->deph = ft_atoi(av[3]);
	else if (ac < 4 || data->deph <= 0)
		data->deph = 1;
	data->ambient_is_set = false;
	data->width = WIDTH;
	data->diff_ok = false;
	data->height = HEIGHT;
	data->has_checker = false;
	data->checker_color = (mlx_color)(uint32_t){0x00000000};
	data->debug = false;
	data->textures = NULL;
	data->thread_running = true;
	data->is_full = false;
	data->sorted_objs = NULL;
	data->bvh_nodes = NULL;
	data->debug_depth = 8;
	data->scene_fd = -1;
	data->scene_line = NULL;
	if (ac == 5)
		data->s_per_pixs = ft_atoi(av[4]);
	else if (ac != 5 || data->s_per_pixs <= 0)
		data->s_per_pixs = 1;
	data->objs = NULL;
	data->light = NULL;
	srand(time(NULL));
	ft_memset(data->key_table, 0, sizeof(data->key_table));
	data->cam.up_guide = (t_vec3){0, 1, 0};
	data->speed = 0.5;
	data->rot_speed = 0.05;
	data->lines = false;
	data->mlx = mlx_init();
	if (!data->mlx)
		clean_exit(data, 1, "Error init MLX\n", 0);
	data->filename = av[1];
	init_thread_p(data);
	read_rt(data);
	convert_list_to_arrays(data);
	if (data->obj_count > 25)
		data->use_bvh = true;
	else
		data->use_bvh = false;
	printf("pars finshed\n");
	calcul_ambient(data);
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		clean_exit(data, 1, "Error Malloc MLX\n", 0);
	data->last_frame_time = get_time();
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

	if (ac < 2 || ac > 5)
	{
		ft_putstr_fd("invalid argument:\n./MiniRT <file.rt>\n Optional : ./MiniRT <file.rt> <sample per pixel> <resolution>\n", 2);
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
	init_data(data, info, av, ac);
	mlx_set_fps_goal(data->mlx, 60);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_down, data);
	mlx_on_event(data->mlx, data->win, MLX_KEYUP, key_up, data);
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook, data);
	mlx_add_loop_hook(data->mlx, update, data);
	mlx_loop(data->mlx);
	clean_exit(data, 0, NULL, 0);
	return (0);
}
