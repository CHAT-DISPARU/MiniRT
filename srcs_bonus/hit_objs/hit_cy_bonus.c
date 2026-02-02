/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cy_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:59:51 by titan             #+#    #+#             */
/*   Updated: 2026/02/02 17:19:08 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

bool	hit_border_cy(t_ray ray, double x_plane, double *t_out)
{
	double	t;
	double	y;
	double	z;

	if (fabs(ray.dir.x) < 0.0001)
		return (false);
	t = (x_plane - ray.origin.x) / ray.dir.x;
	if (t < 0.001)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	z = ray.origin.z + t * ray.dir.z;
	if ((y * y + z * z) > 1.0)
		return (false);
	*t_out = t;
	return (true);
}

/*
	x(t) = d0t + o0
	y(t) = d1t + o1
	z(t) = d2t + o2


	r(t) = O + TD

	y² + z² = r²

	D(d0, d1, d2)
	O(o0, o1, o2)

	(d1t + O1)² + (d2t + o2)²= r²
	d1t² + 2(d1t * o1) + o1² + d2t² + 2(d2t * o2) + o2² - r² = 0
	t²(d1 + d2) + 2t(d1 * o1 + d2 * o2) + 2o2² - r²
*/

t_hit_r	set_rec(t_ray ray, t_cy_utils utils, t_obj *cy, t_hit hit)
{
	t_hit_r	rec;

	rec.t = utils.closest_t;
	rec.p = vec_add(ray.origin, vec_scale(ray.dir, utils.closest_t));
	if (utils.hit_zone == 1)
	{
		hit.local_normal = vec_add(hit.l_ray.origin,
				vec_scale(hit.l_ray.dir, utils.closest_t));
		hit.local_normal.x = 0;
	}
	else if (utils.hit_zone == 2)
		hit.local_normal = (t_vec3){1, 0, 0};
	else
		hit.local_normal = (t_vec3){-1, 0, 0};
	rec.normal = mat4_mult_vec3(&cy->transform, hit.local_normal, 0.0);
	rec.normal = vec_normalize(rec.normal);
	if (vec_dot_scal(ray.dir, rec.normal) > 0)
		rec.normal = vec_scale(rec.normal, -1.0);
	return (rec);
}

void	see_caps(t_hit hit, t_cy_utils *utils)
{
	if (hit_border_cy(hit.l_ray, 1.0, &hit.t))
	{
		if (hit.t < utils->closest_t)
		{
			utils->hit_zone = 2;
			utils->closest_t = hit.t;
		}
	}
	if (hit_border_cy(hit.l_ray, -1.0, &hit.t))
	{
		if (hit.t < utils->closest_t)
		{
			utils->hit_zone = 3;
			utils->closest_t = hit.t;
		}
	}
}

void	see_poly(t_hit hit, t_vec3 poly, double delta, t_cy_utils *utils)
{
	double		hit_x;

	hit.t = (-poly.y - sqrt(delta)) / (poly.x);
	if (hit.t > 0.001)
	{
		hit_x = hit.l_ray.origin.x + hit.t * hit.l_ray.dir.x;
		if (hit_x >= -1.0 && hit_x <= 1.0)
		{
			utils->closest_t = hit.t;
			utils->hit_zone = 1;
		}
	}
	if (utils->hit_zone == 0)
	{
		hit.t = (-poly.y + sqrt(delta)) / poly.x;
		if (hit.t > EPSILON)
		{
			hit_x = hit.l_ray.origin.x + hit.t * hit.l_ray.dir.x;
			if (hit_x >= -1.0 && hit_x <= 1.0)
			{
				utils->closest_t = hit.t;
				utils->hit_zone = 1;
			}
		}
	}
}

bool	hit_cylinder(t_obj *cy, t_ray ray, t_hit_r *rec)
{
	t_vec3		poly;
	double		delta;
	t_hit		hit;
	t_cy_utils	utils;

	utils.closest_t = INFINITY;
	utils.hit_zone = 0;
	hit.l_ray.origin = mat4_mult_vec3(&cy->inverse_transform, ray.origin, 1.0);
	hit.l_ray.dir = mat4_mult_vec3(&cy->inverse_transform, ray.dir, 0.0);
	poly.x = (hit.l_ray.dir.y * hit.l_ray.dir.y)
		+ (hit.l_ray.dir.z * hit.l_ray.dir.z);
	poly.y = ((hit.l_ray.origin.y * hit.l_ray.dir.y)
			+ (hit.l_ray.origin.z * hit.l_ray.dir.z));
	poly.z = (hit.l_ray.origin.y * hit.l_ray.origin.y)
		+ (hit.l_ray.origin.z * hit.l_ray.origin.z) - 1.0;
	delta = (poly.y * poly.y) - (poly.x * poly.z);
	if (delta >= 0)
		see_poly(hit, poly, delta, &utils);
	see_caps(hit, &utils);
	if (utils.hit_zone == 0)
		return (false);
	*rec = set_rec(ray, utils, cy, hit);
	return (true);
}
