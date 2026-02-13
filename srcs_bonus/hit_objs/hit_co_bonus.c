/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_co_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:17:23 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/13 13:19:59 by titan            ###   ########.fr       */
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

void	see_cap(t_hit hit, t_cy_utils *utils, double height, double rad)
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

void	see_poly_co(t_hit hit, t_vec3 poly, double delta, t_cy_utils *utils)
{
	double	t1;
	double	t2;
	double	hit_x;
	bool	v1 = false;
	bool	v2 = false;

	t1 = (-poly.y - sqrt(delta)) / poly.x;
	t2 = (-poly.y + sqrt(delta)) / poly.x;
	if (t1 > EPSILON && t1 < utils->closest_t)
	{
		hit_x = hit.l_ray.origin.x + t1 * hit.l_ray.dir.x;
		if (hit_x >= 0.0 && hit_x <= utils->half_h)
			v1 = true;
	}
	if (t2 > EPSILON && t2 < utils->closest_t)
	{
		hit_x = hit.l_ray.origin.x + t2 * hit.l_ray.dir.x;
		if (hit_x >= 0.0 && hit_x <= utils->half_h)
			v2 = true;
	}
	if (v1 && v2)
	{
		if (t1 < t2)
			utils->closest_t = t1;
		else
			utils->closest_t = t2;
	}
	else if (v1)
		utils->closest_t = t1;
	else if (v2)
		utils->closest_t = t2;
	else
		return ;
	utils->hit_zone = 1;
}

t_hit_r	set_rec_co(t_ray ray, t_cy_utils utils, t_obj *cy, t_hit hit, double k)
{
	t_hit_r	rec;
	t_vec3	local_p;

	rec.obj_ptr = cy;
	rec.color = cy->color;
	rec.t = utils.closest_t;
	rec.p = vec_add(ray.origin, vec_scale(ray.dir, utils.closest_t));
	local_p = vec_add(hit.l_ray.origin, vec_scale(hit.l_ray.dir, utils.closest_t));
	if (utils.hit_zone == 1)
	{
		hit.local_normal = vec_add(hit.l_ray.origin,
				vec_scale(hit.l_ray.dir, utils.closest_t));
		hit.local_normal.x = -k * hit.local_normal.x;
		double phi = atan2(local_p.z, local_p.y);
		rec.u = (phi + PI) / (2.0 * PI);
		rec.v = (local_p.x + 1.0) * 0.5;
	}
	else if (utils.hit_zone == 2)
	{
		hit.local_normal = (t_vec3){1, 0, 0};
		rec.u = (local_p.y + 1.0) * 0.5;
		rec.v = (local_p.z + 1.0) * 0.5;
	}
	rec.normal = mat4_mult_vec3(&cy->transform, hit.local_normal, 0.0);
	rec.normal = vec_normalize(rec.normal);
	if (vec_dot_scal(ray.dir, rec.normal) > 0)
		rec.normal = vec_scale(rec.normal, -1.0);
	return (rec);
}

bool	hit_cone(t_obj *co, t_ray ray, t_hit_r *rec)
{
	t_vec3		poly;
	double		delta;
	t_hit		hit;
	t_cy_utils	utils;
	double		k = pow(co->rad_1, 2) / pow(co->height, 2);

	utils.closest_t = rec->t;
	utils.hit_zone = 0;
	utils.half_h = co->height;
	hit.l_ray.origin = mat4_mult_vec3(&co->inverse_transform, ray.origin, 1.0);
	hit.l_ray.dir = mat4_mult_vec3(&co->inverse_transform, ray.dir, 0.0);
	poly.x = -k * (hit.l_ray.dir.x * hit.l_ray.dir.x) + (hit.l_ray.dir.y * hit.l_ray.dir.y)
		+ (hit.l_ray.dir.z * hit.l_ray.dir.z);
	poly.y = -k * (hit.l_ray.origin.x * hit.l_ray.dir.x)+ ((hit.l_ray.origin.y * hit.l_ray.dir.y)
			+ (hit.l_ray.origin.z * hit.l_ray.dir.z));
	poly.z = -k * (hit.l_ray.origin.x * hit.l_ray.origin.x) + (hit.l_ray.origin.y * hit.l_ray.origin.y)
		+ (hit.l_ray.origin.z * hit.l_ray.origin.z);
	delta = (poly.y * poly.y) - (poly.x * poly.z);
	if (delta >= 0)
		see_poly_co(hit, poly, delta, &utils);
	see_cap(hit, &utils, utils.half_h, pow(co->rad_1, 2));
	if (utils.hit_zone == 0)
		return (false);
	*rec = set_rec_co(ray, utils, co, hit, k);
	return (true);
}
