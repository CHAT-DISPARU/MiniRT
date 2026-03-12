/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_part4_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:08:05 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 11:08:23 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	do_opacity_refract2(t_op_ni	*op_ni, t_hit_r *rec, t_ray ray)
{
	op_ni->re_col = (t_vec3){0, 0, 0};
	op_ni->rea_col = (t_vec3){0, 0, 0};
	op_ni->cos_theta_i = vec_dot_scal(ray.dir, rec->normal);
	if (op_ni->cos_theta_i < 0.0)
	{
		op_ni->is_inside = false;
		op_ni->ot_n = rec->normal;
		op_ni->cos_theta_i = -op_ni->cos_theta_i;
		op_ni->eta_i = 1.0;
		op_ni->eta_t = rec->obj_ptr->ni;
	}
	else
	{
		op_ni->is_inside = true;
		op_ni->ot_n = vec_scale(rec->normal, -1.0);
		op_ni->eta_i = rec->obj_ptr->ni;
		op_ni->eta_t = 1.0;
	}
	op_ni->eta_ratio = op_ni->eta_i / op_ni->eta_t;
	op_ni->r0 = (op_ni->eta_i - op_ni->eta_t) / (op_ni->eta_i + op_ni->eta_t);
	op_ni->r0 = op_ni->r0 * op_ni->r0;
	op_ni->fresnel = op_ni->r0 + (1.0 - op_ni->r0)
		* pow(1.0 - op_ni->cos_theta_i, 5.0);
}

t_vec3	do_opacity_refract(t_data *data, t_hit_r *rec, int deph, t_ray ray)
{
	t_op_ni	op_ni;

	do_opacity_refract2(&op_ni, rec, ray);
	op_ni.re_ray.dir = vec_normalize(vec_sub(ray.dir,
				vec_scale(op_ni.ot_n, 2.0
					* vec_dot_scal(ray.dir, op_ni.ot_n))));
	op_ni.re_ray.origin = vec_add(rec->p, vec_scale(op_ni.ot_n, EPSILON));
	op_ni.re_col = check_hit(data, op_ni.re_ray, deph - 1);
	op_ni.sin2_theta_t = op_ni.eta_ratio * op_ni.eta_ratio
		* (1.0 - op_ni.cos_theta_i * op_ni.cos_theta_i);
	if (op_ni.sin2_theta_t > 1.0)
		op_ni.fresnel = 1.0;
	else
	{
		op_ni.cos_theta_t = sqrt(1.0 - op_ni.sin2_theta_t);
		op_ni.r_dir_part1 = vec_scale(ray.dir, op_ni.eta_ratio);
		op_ni.r_dir_part2 = vec_scale(op_ni.ot_n,
				op_ni.eta_ratio * op_ni.cos_theta_i - op_ni.cos_theta_t);
		op_ni.rea_ray.dir = vec_normalize(vec_add(op_ni.r_dir_part1,
					op_ni.r_dir_part2));
		op_ni.rea_ray.origin = vec_sub(rec->p, vec_scale(op_ni.ot_n, EPSILON));
		op_ni.re_col = check_hit(data, op_ni.rea_ray, deph - 1);
	}
	return (tint_color_acc(op_ni.fresnel, op_ni.rea_col, op_ni.re_col, rec));
}

t_vec3	check_hit(t_data *data, t_ray ray, int deph)
{
	t_vec3		color_acc;
	t_hit_r		rec;

	color_acc = (t_vec3){0, 0, 0};
	if (deph <= 0)
		return (color_acc);
	ft_bzero(&rec, sizeof(t_hit_r));
	rec.t = DBL_MAX;
	if (hit_someting(data, ray, &rec))
	{
		set_hit_col(&rec, data);
		light_hit(&rec, data, &color_acc, ray);
		if (deph > 1)
		{
			if (rec.obj_ptr->opacity < 1)
				color_acc = vec_add(do_opacity_refract(data, &rec, deph, ray),
						color_acc);
			else
				color_acc = vec_add(color_acc,
						rought_reflect(data, &rec, ray, deph));
		}
	}
	debug_bvh(&color_acc, data, &rec, ray);
	return (color_acc);
}
