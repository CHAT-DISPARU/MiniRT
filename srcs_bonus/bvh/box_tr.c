/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_tr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:22:46 by titan             #+#    #+#             */
/*   Updated: 2026/02/11 11:23:18 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_aabb	aabb_triangle(t_obj *tr)
{
    t_aabb	box;

    box = empty_aabb();
    add_point_to_aabb(&box, tr->tri.p1);
    add_point_to_aabb(&box, tr->tri.p2);
    add_point_to_aabb(&box, tr->tri.p3);
    box.min = vec_sub(box.min, (t_vec3){EPSILON, EPSILON, EPSILON});
    box.max = vec_add(box.max, (t_vec3){EPSILON, EPSILON, EPSILON});
    return (box);
}
