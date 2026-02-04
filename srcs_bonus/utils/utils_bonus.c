/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:17:57 by titan             #+#    #+#             */
/*   Updated: 2026/02/04 12:27:56 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	ft_facesize(t_face_c *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	ft_faceadd_back(t_face_c **lst, t_face_c *new)
{
	t_face_c	*current;

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

void	ft_lightadd_back(t_light **lst, t_light *new)
{
	t_light	*current;

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
	functions[CALC_TR] = hit_triangle;
	functions[CALC_HY] = hit_hyperboloid;
	functions[CALC_CO] = hit_cone;
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
