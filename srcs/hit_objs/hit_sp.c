/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:59:41 by titan             #+#    #+#             */
/*   Updated: 2026/01/31 23:28:18 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>
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
	t_vec3	local_normal;

	l_ray.origin = mat4_mult_vec3(&sp->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&sp->inverse_transform, ray.dir, 0.0);
	poly.x = vec_dot_scal(l_ray.dir, l_ray.dir);
	poly.y = vec_dot_scal(l_ray.origin, l_ray.dir);
	poly.z = vec_dot_scal(l_ray.origin, l_ray.origin) - 1.0;
	delta = (poly.y * poly.y) - (poly.x * poly.z);
	if (delta < 0)
		return (false);
	rec->t = (-poly.y - sqrt(delta)) / (poly.x);
	if (rec->t < 0.001)
	{
		rec->t = (-poly.y + sqrt(delta)) / (poly.x);
		if (rec->t < 0.001)
			return (false);
	}
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, rec->t));
	local_normal = vec_add(l_ray.origin, vec_scale(l_ray.dir, rec->t));
	rec->normal = mat4_mult_vec3(&sp->transform, local_normal, 0);
	rec->normal = vec_normalize(rec->normal);
	return (true);
}
