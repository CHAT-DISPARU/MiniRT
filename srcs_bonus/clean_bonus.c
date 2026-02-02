/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:10:27 by titan             #+#    #+#             */
/*   Updated: 2026/02/02 16:14:15 by gajanvie         ###   ########.fr       */
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

void	clean_exit(t_data *data, int exit_code, char *mess_eror, int i)
{
	t_obj	*tmp;
	t_light	*tmp2;

	print_m_and_gnl(data, mess_eror, exit_code, i);
	while (data->objs)
	{
		tmp = data->objs->next;
		free(data->objs);
		data->objs = tmp;
	}
	while (data->light)
	{
		tmp2 = data->light->next;
		free(data->light);
		data->light = tmp2;
	}
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
