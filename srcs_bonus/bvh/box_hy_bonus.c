/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_hy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:35:35 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 13:23:24 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	aabb_hyperboloid(t_obj *hy)
{
	t_aabb	box;
	double	max_r;
	double	half_h;

	max_r = fmax(hy->rad_1, hy->rad_2);
	half_h = hy->height / 2.0;
	box.min = (t_vec3){-max_r, -max_r, -half_h};
	box.max = (t_vec3){max_r, max_r, half_h};
	return (aabb_transform_matrix(box, hy->transform));
}
