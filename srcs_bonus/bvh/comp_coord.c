/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_coord.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:04:52 by titan             #+#    #+#             */
/*   Updated: 2026/02/11 14:07:00 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	cmp_z(const void *a, const void *b)
{
	t_obj	*o1;
	t_obj	*o2;
	double	z1;
	double	z2;

	o1 = *(t_obj **)a;
	o2 = *(t_obj **)b;
	z1 = get_aabb_by_type(o1).min.z;
	z2 = get_aabb_by_type(o2).min.z;
	if (z1 > z2)
		return (1);
	else
		return (-1);
}

int	cmp_y(const void *a, const void *b)
{
	t_obj	*o1;
	t_obj	*o2;
	double	y1;
	double	y2;

	o1 = *(t_obj **)a;
	o2 = *(t_obj **)b;
	y1 = get_aabb_by_type(o1).min.y;
	y2 = get_aabb_by_type(o2).min.y;
	if (y1 > y2)
		return (1);
	else
		return (-1);
}

int	cmp_x(const void *a, const void *b)
{
	t_obj	*o1;
	t_obj	*o2;
	double	x1;
	double	x2;

	o1 = *(t_obj **)a;
	o2 = *(t_obj **)b;
	x1 = get_aabb_by_type(o1).min.x;
	x2 = get_aabb_by_type(o2).min.x;
	if (x1 > x2)
		return (1);
	else
		return (-1);
}
