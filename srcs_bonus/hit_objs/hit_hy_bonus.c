/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_hy_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:43:40 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/03 17:30:50 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

/*
	x2/a2 ± y2/b2 − z2/c2 = 1
	a = 1;
	b = 1;
	c = 1;

	(d0t + oO)²  ± (d1t + o1)² - (d2t + o2)² = 1
	[d0t² + 2*d0t*oO + oO²]  ± [d1t² + 2*d1t*o1 + o1²] - [d2t² + 2*d2t*o2 + o2²] = 1

	t²(d0 ± d1 - d2) +
	2t(d0 * o0 ± d1 * o1 - d2 * o2) +
	o0² ± o1² - o2² -1 = 0
*/

bool	hit_hyperboloid(t_obj *hy, t_ray ray, t_hit_r *rec)
{
	t_hit		hit;
	double		a;
	double		b;
	double		c;
	double		delta;

	hit.l_ray.origin = mat4_mult_vec3(&hy->inverse_transform, ray.origin, 1.0);
	hit.l_ray.dir = mat4_mult_vec3(&hy->inverse_transform, ray.dir, 0.0);
	a = (hit.l_ray.dir.x + hit.l_ray.dir.y - hit.l_ray.dir.z) * (hit.l_ray.dir.x + hit.l_ray.dir.y - hit.l_ray.dir.z);
	b = (hit.l_ray.dir.x * hit.l_ray.origin.x + hit.l_ray.dir.y * hit.l_ray.origin.y - hit.l_ray.dir.z * hit.l_ray.origin.z);
	c = (hit.l_ray.origin.x * hit.l_ray.origin.x) + (hit.l_ray.origin.y * hit.l_ray.origin.y) - (hit.l_ray.origin.z * hit.l_ray.origin.z) - 1;
	delta = (b * b) - (a * c);
	if (delta < 0)
	{
		/*a = (hit.l_ray.dir.x - hit.l_ray.dir.y + hit.l_ray.dir.z) * (hit.l_ray.dir.x - hit.l_ray.dir.y + hit.l_ray.dir.z);
		b = (hit.l_ray.dir.x * hit.l_ray.origin.x - hit.l_ray.dir.y * hit.l_ray.origin.y - hit.l_ray.dir.z * hit.l_ray.origin.z);
		c = (hit.l_ray.origin.x * hit.l_ray.origin.x) - (hit.l_ray.origin.y * hit.l_ray.origin.y) - (hit.l_ray.origin.z * hit.l_ray.origin.z) - 1;
		delta = (b * b) - (a * c);
		if (delta < 0)*/
		return (false);
	}
	rec->t = (-b - sqrt(delta)) / (a);
	if (rec->t < EPSILON)
	{
		rec->t = (-b + sqrt(delta)) / (a);
		if (rec->t < EPSILON)
			return (false);
	}
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, rec->t));
	if (fabs(rec->p.y) > 1.0 || fabs(rec->p.y) < -1.0)
		return (false);
	hit.local_normal = vec_add(hit.l_ray.origin, vec_scale(hit.l_ray.dir, rec->t));
	rec->normal = mat4_mult_vec3(&hy->transform, hit.local_normal, 0);
	rec->normal = vec_normalize(rec->normal);
	return (true);
}
