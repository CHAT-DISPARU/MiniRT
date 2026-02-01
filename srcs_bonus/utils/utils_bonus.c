/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:17:57 by titan             #+#    #+#             */
/*   Updated: 2026/02/01 13:58:38 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	ft_objadd_back(t_obj **lst, t_obj *new)
{
	t_obj	*current;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	init_t_calc_f(t_calc_f *functions)
{
	functions[CALC_SQ] = hit_square;
	functions[CALC_CY] = hit_cylinder;
	functions[CALC_PL] = hit_plane;
	functions[CALC_SP] = hit_sphere;
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
	thread_calls(data);
	return (0);
}
