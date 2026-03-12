/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_co.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 13:20:06 by titan             #+#    #+#             */
/*   Updated: 2026/02/11 13:20:39 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	aabb_cone(t_obj *co)
{
	t_aabb	local_box;
	double	r;
	double	h;

	r = co->rad_1;
	h = co->height;
	local_box.min = (t_vec3){0.0, -r, -r};
	local_box.max = (t_vec3){h, r, r};
	return (aabb_transform_matrix(local_box, co->transform));
}
