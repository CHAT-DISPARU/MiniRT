/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_cy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:25:47 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 13:30:14 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	aabb_cylinder(t_obj *cy)
{
	t_aabb	box;

	box.min = (t_vec3){-1.0, -1.0, -1.0};
	box.max = (t_vec3){1.0, 1.0, 1.0};
	return (aabb_transform_matrix(box, cy->transform));
}
