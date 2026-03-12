/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_co_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:17:23 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:47:13 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	see_poly_co(t_hit hit, t_vec3 poly, double delta, t_cy_utils *utils)
{
	t_poly_co	co_p;
	double		hit_x;

	co_p.v1 = false;
	co_p.v2 = false;
	co_p.t1 = (-poly.y - sqrt(delta)) / poly.x;
	co_p.t2 = (-poly.y + sqrt(delta)) / poly.x;
	if (co_p.t1 > EPSILON && co_p.t1 < utils->closest_t)
	{
		hit_x = hit.l_ray.origin.x + co_p.t1 * hit.l_ray.dir.x;
		if (hit_x >= 0.0 && hit_x <= utils->half_h)
			co_p.v1 = true;
	}
	if (co_p.t2 > EPSILON && co_p.t2 < utils->closest_t)
	{
		hit_x = hit.l_ray.origin.x + co_p.t2 * hit.l_ray.dir.x;
		if (hit_x >= 0.0 && hit_x <= utils->half_h)
			co_p.v2 = true;
	}
	if (set_closest_co(utils, &co_p) == false)
		return ;
	utils->hit_zone = 1;
}

void	set_rec_co2(t_hit *hit, t_hit_r *rec, t_vec3 local_p)
{
	hit->local_normal = (t_vec3){1, 0, 0};
	rec->u = (local_p.y + 1.0) * 0.5;
	rec->v = (local_p.z + 1.0) * 0.5;
}

t_hit_r	set_rec_co(t_ray ray, t_cy_utils utils, t_obj *cy, t_hit hit)
{
	t_hit_r	rec;
	t_vec3	lp;
	double	phi;

	rec.obj_ptr = cy;
	rec.color = cy->color;
	rec.t = utils.closest_t;
	rec.p = vec_add(ray.origin, vec_scale(ray.dir, utils.closest_t));
	lp = vec_add(hit.l_ray.origin, vec_scale(hit.l_ray.dir, utils.closest_t));
	if (utils.hit_zone == 1)
	{
		hit.local_normal = vec_add(hit.l_ray.origin,
				vec_scale(hit.l_ray.dir, utils.closest_t));
		hit.local_normal.x = -hit.k * hit.local_normal.x;
		phi = atan2(lp.z, lp.y);
		rec.u = (phi + PI) / (2.0 * PI);
		rec.v = (lp.x + 1.0) * 0.5;
	}
	else if (utils.hit_zone == 2)
		set_rec_co2(&hit, &rec, lp);
	rec.normal = mat4_mult_vec3(&cy->transform, hit.local_normal, 0.0);
	rec.normal = vec_normalize(rec.normal);
	if (vec_dot_scal(ray.dir, rec.normal) > 0)
		rec.normal = vec_scale(rec.normal, -1.0);
	return (rec);
}

void	calc_cone_poly(t_vec3 *poly, t_hit hit)
{
	poly->x = -hit.k * (hit.l_ray.dir.x * hit.l_ray.dir.x)
		+ (hit.l_ray.dir.y * hit.l_ray.dir.y)
		+ (hit.l_ray.dir.z * hit.l_ray.dir.z);
	poly->y = -hit.k * (hit.l_ray.origin.x * hit.l_ray.dir.x)
		+ ((hit.l_ray.origin.y * hit.l_ray.dir.y)
			+ (hit.l_ray.origin.z * hit.l_ray.dir.z));
	poly->z = -hit.k * (hit.l_ray.origin.x * hit.l_ray.origin.x)
		+ (hit.l_ray.origin.y * hit.l_ray.origin.y)
		+ (hit.l_ray.origin.z * hit.l_ray.origin.z);
}

bool	hit_cone(t_obj *co, t_ray ray, t_hit_r *rec)
{
	t_vec3		poly;
	double		delta;
	t_hit		hit;
	t_cy_utils	utils;

	hit.k = pow(co->rad_1, 2) / pow(co->height, 2);
	utils.closest_t = rec->t;
	utils.hit_zone = 0;
	utils.half_h = co->height;
	hit.l_ray.origin = mat4_mult_vec3(&co->inverse_transform, ray.origin, 1.0);
	hit.l_ray.dir = mat4_mult_vec3(&co->inverse_transform, ray.dir, 0.0);
	calc_cone_poly(&poly, hit);
	delta = (poly.y * poly.y) - (poly.x * poly.z);
	if (delta >= 0)
		see_poly_co(hit, poly, delta, &utils);
	see_cap_co(hit, &utils, utils.half_h, pow(co->rad_1, 2));
	if (utils.hit_zone == 0)
		return (false);
	*rec = set_rec_co(ray, utils, co, hit);
	return (true);
}
