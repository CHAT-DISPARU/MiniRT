/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_co_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:46:19 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:46:46 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

bool	hit_border_co(t_ray ray, double x_plane, double *t_out, double rad)
{
	double	t;
	double	y;
	double	z;

	if (fabs(ray.dir.x) < EPSILON)
		return (false);
	t = (x_plane - ray.origin.x) / ray.dir.x;
	if (t < EPSILON)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	z = ray.origin.z + t * ray.dir.z;
	if ((y * y + z * z) > rad)
		return (false);
	*t_out = t;
	return (true);
}

void	see_cap_co(t_hit hit, t_cy_utils *utils, double height, double rad)
{
	if (hit_border_co(hit.l_ray, height, &hit.t, rad))
	{
		if (hit.t > EPSILON && hit.t < utils->closest_t)
		{
			utils->hit_zone = 2;
			utils->closest_t = hit.t;
		}
	}
}

bool	set_closest_co(t_cy_utils *utils, t_poly_co *co_p)
{
	if (co_p->v1 && co_p->v2)
	{
		if (co_p->t1 < co_p->t2)
			utils->closest_t = co_p->t1;
		else
			utils->closest_t = co_p->t2;
	}
	else if (co_p->v1)
		utils->closest_t = co_p->t1;
	else if (co_p->v2)
		utils->closest_t = co_p->t2;
	else
		return (false);
	return (true);
}
