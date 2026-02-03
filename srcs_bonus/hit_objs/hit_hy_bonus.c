/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_hy_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:43:40 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/03 23:40:39 by titan            ###   ########.fr       */
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

	
	f(x,y,z)=x2+y2−kz2−rmin2​=0
	X : La dérivée de x2  2x.
	Y : La dérivée de y2 2y.
	Z : La dérivée de −kz2 −2kz
*/

bool	hit_hyperboloid(t_obj *hy, t_ray ray, t_hit_r *rec)
{
	t_hit   hit;
	t_vec3  oc, dir;
	double  a, b, c, delta;
	double  t1, t2, t_final;
	bool    v1, v2;
	double r_min = hy->rad_1;
	double r_max = hy->rad_2;
	double h     = hy->height;
	double half_h = h / 2.0;
	double k = (pow(r_max, 2) - pow(r_min, 2)) / pow(half_h, 2);

	hit.l_ray.origin = mat4_mult_vec3(&hy->inverse_transform, ray.origin, 1.0);
	hit.l_ray.dir = mat4_mult_vec3(&hy->inverse_transform, ray.dir, 0.0);
	dir = hit.l_ray.dir;
	oc = hit.l_ray.origin;
	a = (dir.x * dir.x) + (dir.y * dir.y) - k * (dir.z * dir.z);
	b = (oc.x * dir.x) + (oc.y * dir.y) - k * (oc.z * dir.z);
	c = (oc.x * oc.x) + (oc.y * oc.y) - k * (oc.z * oc.z) - (r_min * r_min);
	delta = (b * b) - (a * c);
	if (delta < 0)
		return (false);
	t1 = (-b - sqrt(delta)) / a;
	t2 = (-b + sqrt(delta)) / a;
	v1 = false;
	v2 = false;
	if (t1 > EPSILON)
	{
		double z = oc.z + t1 * dir.z;
		if (fabs(z) <= half_h)
			v1 = true;
	}
	if (t2 > EPSILON)
	{
		double z = oc.z + t2 * dir.z;
		if (fabs(z) <= half_h)
			v2 = true;
	}
	if (v1 && v2)
	{
		if (t1 < t2)
			t_final = t1;
		else
			t_final = t2;
	}
	else if (v1)
		t_final = t1;
	else if (v2)
		t_final = t2;
	else
		return (false);
	rec->t = t_final;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, rec->t));
	t_vec3 local_hit = vec_add(oc, vec_scale(dir, rec->t));
	t_vec3 local_normal;
	local_normal.x = local_hit.x;
	local_normal.y = local_hit.y;
	local_normal.z = -k * local_hit.z;
	rec->normal = mat4_mult_vec3(&hy->transform, local_normal, 0.0);
	rec->normal = vec_normalize(rec->normal);
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	return (true);
}
