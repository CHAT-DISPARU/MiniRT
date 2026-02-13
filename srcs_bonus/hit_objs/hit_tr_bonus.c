/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_tr_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:42:25 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/13 13:13:32 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

bool	hit_triangle(t_obj *tr, t_ray ray, t_hit_r *rec)
{
	t_vec3  edge1;
	t_vec3	edge2;
	t_vec3  pvec;
	t_vec3	tvec;
	t_vec3	qvec;
	double  det;
	double  u;
	double	v;
	double	t;

	edge1 = vec_sub(tr->tri.p2, tr->tri.p1);
	edge2 = vec_sub(tr->tri.p3, tr->tri.p1);
	pvec = vec_cross(ray.dir, edge2);
	det = vec_dot_scal(edge1, pvec);
	if (det > -EPSILON && det < EPSILON)
		return (false);
	det = 1.0 / det;
	tvec = vec_sub(ray.origin, tr->tri.p1);
	u = vec_dot_scal(tvec, pvec) * det;
	if (u < 0.0 || u > 1.0)
		return (false);
	qvec = vec_cross(tvec, edge1);
	v = vec_dot_scal(ray.dir, qvec) * det;
	if (v < 0.0 || u + v > 1.0)
		return (false);
	t = vec_dot_scal(edge2, qvec) * det;
	if (t > EPSILON && t < rec->t)
	{
		rec->t = t;
		rec->p = vec_add(ray.origin, vec_scale(ray.dir, t));
		rec->normal = tr->tri.normal;
		if (vec_dot_scal(ray.dir, rec->normal) > 0)
			rec->normal = vec_scale(rec->normal, -1.0);
		rec->u = u;
        rec->v = v;
		rec->color = tr->color;
		rec->obj_ptr = tr;
		return (true);
	}
	return (false);
}
