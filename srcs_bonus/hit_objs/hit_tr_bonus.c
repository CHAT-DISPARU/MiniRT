/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_tr_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:42:25 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/06 15:45:53 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_rec_tr(t_hit_r *rec, t_ray ray, t_obj *tr, t_bi_tr tr_b)
{
	double	w;

	rec->t = tr_b.t;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, tr_b.t));
	rec->normal = tr->tri.normal;
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	w = 1.0 - tr_b.u - tr_b.v;
	rec->u = (w * tr->tri.uv1.x) + (tr_b.u * tr->tri.uv2.x) + (tr_b.v * tr->tri.uv3.x);
	rec->v = (w * tr->tri.uv1.y) + (tr_b.u * tr->tri.uv2.y) + (tr_b.v * tr->tri.uv3.y);
	rec->color = tr->color;
	rec->obj_ptr = tr;
}

bool	hit_triangle(t_obj *tr, t_ray ray, t_hit_r *rec)
{
	t_bi_tr	tr_b;

	tr_b.edge1 = vec_sub(tr->tri.p2, tr->tri.p1);
	tr_b.edge2 = vec_sub(tr->tri.p3, tr->tri.p1);
	tr_b.pvec = vec_cross(ray.dir, tr_b.edge2);
	tr_b.det = vec_dot_scal(tr_b.edge1, tr_b.pvec);
	if (tr_b.det > -EPSILON && tr_b.det < EPSILON)
		return (false);
	tr_b.det = 1.0 / tr_b.det;
	tr_b.tvec = vec_sub(ray.origin, tr->tri.p1);
	tr_b.u = vec_dot_scal(tr_b.tvec, tr_b.pvec) * tr_b.det;
	if (tr_b.u < 0.0 || tr_b.u > 1.0)
		return (false);
	tr_b.qvec = vec_cross(tr_b.tvec, tr_b.edge1);
	tr_b.v = vec_dot_scal(ray.dir, tr_b.qvec) * tr_b.det;
	if (tr_b.v < 0.0 || tr_b.u + tr_b.v > 1.0)
		return (false);
	tr_b.t = vec_dot_scal(tr_b.edge2, tr_b.qvec) * tr_b.det;
	if (tr_b.t > EPSILON && tr_b.t < rec->t)
	{
		set_rec_tr(rec, ray, tr, tr_b);
		return (true);
	}
	return (false);
}
