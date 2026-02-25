/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sp_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:59:41 by titan             #+#    #+#             */
/*   Updated: 2026/02/13 13:08:13 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>
/*
	D direction O origine t inconue

	Ray(t) = o + t * D;

	tous les point f d'une sphere

	||f - c|| = r²; longeur vecteur au carre
	est strictement egual au produit scalire de lui meme
	(f - c) * (f - c) = r²
	((O + t * D) - C) * ((O + t * D) - C) = r²
	(OC + TD) * (OC + TD) = r²

	(a + b)² = a² + 2ab + b²
	t = -l_ray.origin.y / denom;

	(OC * OC) + 2(OC * TD) + TD² = r²
	(OC * OC) + 2t(OC * D) + t²(D * D) = r²
	(OC * OC) + 2t(OC * D) + t²(D * D) - r² = 0
*/

bool	hit_sphere(t_obj *sp, t_ray ray, t_hit_r *rec)
{
	t_ray	l_ray;
	t_vec3	poly;
	double	delta;
	double	t;
	t_vec3	local_normal;

	l_ray.origin = mat4_mult_vec3(&sp->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&sp->inverse_transform, ray.dir, 0.0);
	poly.x = vec_dot_scal(l_ray.dir, l_ray.dir);
	poly.y = vec_dot_scal(l_ray.origin, l_ray.dir);
	poly.z = vec_dot_scal(l_ray.origin, l_ray.origin) - 1.0;
	delta = (poly.y * poly.y) - (poly.x * poly.z);
	if (delta < 0)
		return (false);
	t = (-poly.y - sqrt(delta)) / (poly.x);
	if (t < EPSILON || t > rec->t)
	{
		t = (-poly.y + sqrt(delta)) / (poly.x);
		if (t < EPSILON || t > rec->t)
			return (false);
	}
	rec->t = t;
	rec->obj_ptr = sp;
	rec->color = sp->color;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, rec->t));
	local_normal = vec_add(l_ray.origin, vec_scale(l_ray.dir, rec->t));
	get_sphere_uv(vec_normalize(local_normal), &rec->u, &rec->v);
	rec->normal = mat4_mult_vec3(&sp->transform, local_normal, 0);
	rec->normal = vec_normalize(rec->normal);
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	return (true);
}
