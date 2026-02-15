/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_cost.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:02:25 by titan             #+#    #+#             */
/*   Updated: 2026/02/15 15:40:19 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

double	get_aabb_surface_area(t_aabb box)
{
	t_vec3	size;

	size = vec_sub(box.max, box.min);
	return (2.0 * ((size.x * size.y) + (size.y * size.z) + (size.z * size.x)));
}

/*
	Surface Area Heuristic (SAH)
	Cout=(Aire Gauche ​× NGauche​) + (Aire Droite​ × NDroite​)
*/

int	find_best_split_sah(t_data *data, int start, int count)
{
	int		best_mid = count / 2;
	double	min_cost = DBL_MAX;
	double	current_cost;
	t_aabb	left_box;
	t_aabb	right_box;
	double	area_l, area_r;
	int		i;

	int step = 1; 
	if (count > 32)
		step = count / 10;
	i = 1;
	while (i < count)
	{
		left_box = compute_bounds(&data->sorted_objs[start], i);
		right_box = compute_bounds(&data->sorted_objs[start + i], count - i);
		area_l = get_aabb_surface_area(left_box);
		area_r = get_aabb_surface_area(right_box);
		current_cost = (area_l * i) + (area_r * (count - i));
		if (current_cost < min_cost)
		{
			min_cost = current_cost;
			best_mid = i;
		}
		i += step;
	}
	return (best_mid);
}
