/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_sq.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 13:11:56 by titan             #+#    #+#             */
/*   Updated: 2026/02/11 13:12:48 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	aabb_square(t_obj *sq)
{
	t_aabb	local_box;
	double	thickness;

	thickness = 0.001;
	local_box.min = (t_vec3){-1.0, -thickness, -1.0};
	local_box.max = (t_vec3){1.0, thickness, 1.0};
	return (aabb_transform_matrix(local_box, sq->transform));
}