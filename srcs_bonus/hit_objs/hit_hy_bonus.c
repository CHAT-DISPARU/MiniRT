/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_hy_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:43:40 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/10 13:32:00 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

/*
	x2/a2 ± y2/b2 − z2/c2 = 1
	a = 1;
	b = 1;
	c = 1;

	(d0t + oO)²  ± (d1t + o1)² - (d2t + o2)² = 1
	[d0t² + 2*d0t*oO + oO²]  ± [d1t² + 2*d1t*o1 + o1²]
		- [d2t² + 2*d2t*o2 + o2²] = 1

	t²(d0 ± d1 - d2) +
	2t(d0 * o0 ± d1 * o1 - d2 * o2) +
	o0² ± o1² - o2² -1 = 0

	
	f(x,y,z)=x2+y2−kz2−rmin2​=0
	X : La dérivée de x2  2x.
	Y : La dérivée de y2 2y.
	Z : La dérivée de −kz2 −2kz
*/
void	set_vars(t_hy_utils *hy_u, t_obj *hy, t_ray ray)
{
	hy_u->co_p.v1 = false;
	hy_u->co_p.v2 = false;
	hy_u->r_min = hy->rad_1;
	hy_u->r_max = hy->rad_2;
	hy_u->h = hy->height;
	hy_u->half_h = hy_u->h / 2.0;
	hy_u->k = (pow(hy_u->r_max, 2) - pow(hy_u->r_min, 2))
		/ pow(hy_u->half_h, 2);
	hy_u->hit.l_ray.origin = mat4_mult_vec3(&hy->inverse_transform,
			ray.origin, 1.0);
	hy_u->hit.l_ray.dir = mat4_mult_vec3(&hy->inverse_transform, ray.dir, 0.0);
	hy_u->dir = hy_u->hit.l_ray.dir;
	hy_u->oc = hy_u->hit.l_ray.origin;
	hy_u->poly.x = (hy_u->dir.x * hy_u->dir.x) + (hy_u->dir.y * hy_u->dir.y)
		- hy_u->k * (hy_u->dir.z * hy_u->dir.z);
	hy_u->poly.y = (hy_u->oc.x * hy_u->dir.x) + (hy_u->oc.y * hy_u->dir.y)
		- hy_u->k * (hy_u->oc.z * hy_u->dir.z);
	hy_u->poly.z = (hy_u->oc.x * hy_u->oc.x) + (hy_u->oc.y * hy_u->oc.y)
		- hy_u->k * (hy_u->oc.z * hy_u->oc.z) - (hy_u->r_min * hy_u->r_min);
	hy_u->delta = (hy_u->poly.y * hy_u->poly.y) - (hy_u->poly.x * hy_u->poly.z);
}

void	fuck_norm(t_hy_utils *hy_u)
{
	if (hy_u->co_p.t1 < hy_u->co_p.t2)
		hy_u->co_p.t_final = hy_u->co_p.t1;
	else
		hy_u->co_p.t_final = hy_u->co_p.t2;
}

bool	calc_sols(t_hy_utils *hy_u, t_hit_r *rec)
{
	if (hy_u->co_p.t1 > EPSILON && hy_u->co_p.t1 < rec->t)
	{
		hy_u->z = hy_u->oc.z + hy_u->co_p.t1 * hy_u->dir.z;
		if (fabs(hy_u->z) <= hy_u->half_h)
			hy_u->co_p.v1 = true;
	}
	if (hy_u->co_p.t2 > EPSILON && hy_u->co_p.t2 < rec->t)
	{
		hy_u->z = hy_u->oc.z + hy_u->co_p.t2 * hy_u->dir.z;
		if (fabs(hy_u->z) <= hy_u->half_h)
			hy_u->co_p.v2 = true;
	}
	if (hy_u->co_p.v1 && hy_u->co_p.v2)
		fuck_norm(hy_u);
	else if (hy_u->co_p.v1)
		hy_u->co_p.t_final = hy_u->co_p.t1;
	else if (hy_u->co_p.v2)
		hy_u->co_p.t_final = hy_u->co_p.t2;
	else
		return (false);
	return (true);
}

bool	hit_hyperboloid(t_obj *hy, t_ray ray, t_hit_r *rec)
{
	t_hy_utils	hy_u;

	set_vars(&hy_u, hy, ray);
	if (hy_u.delta < 0)
		return (false);
	hy_u.co_p.t1 = (-hy_u.poly.y - sqrt(hy_u.delta)) / hy_u.poly.x;
	hy_u.co_p.t2 = (-hy_u.poly.y + sqrt(hy_u.delta)) / hy_u.poly.x;
	if (calc_sols(&hy_u, rec) == false)
		return (false);
	rec->color = hy->color;
	rec->t = hy_u.co_p.t_final;
	rec->obj_ptr = hy;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, rec->t));
	hy_u.local_hit = vec_add(hy_u.oc, vec_scale(hy_u.dir, rec->t));
	hy_u.phi = atan2(hy_u.local_hit.y, hy_u.local_hit.x);
	rec->u = (hy_u.phi + PI) / (2.0 * PI);
	rec->v = (hy_u.local_hit.z + hy_u.half_h) / hy_u.h;
	hy_u.local_normal.x = hy_u.local_hit.x;
	hy_u.local_normal.y = hy_u.local_hit.y;
	hy_u.local_normal.z = -hy_u.k * hy_u.local_hit.z;
	rec->normal = mat4_mult_vec3(&hy->transform, hy_u.local_normal, 0.0);
	rec->normal = vec_normalize(rec->normal);
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	return (true);
}
