/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_sp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:19:51 by titan             #+#    #+#             */
/*   Updated: 2026/02/11 13:18:49 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	aabb_sphere(t_obj *sp)
{
	t_aabb	box;

	box.min = (t_vec3){-1, -1, -1};
	box.max = (t_vec3){1, 1, 1};
	return (aabb_transform_matrix(box, sp->transform));
}
