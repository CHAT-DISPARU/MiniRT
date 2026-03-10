/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:57:53 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 14:06:30 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

/*
	rebond
	R=I−2(I⋅N)N
*/

t_ray	calc_ray(t_render_v rv, t_data *data)
{
	t_vec3	local_dir;
	t_ray	ray;

	local_dir.x = (rv.u * data->view_port.viewport_width)
		- (data->view_port.viewport_width / 2);
	local_dir.y = (data->view_port.viewport_height / 2)
		- (rv.v * data->view_port.viewport_height);
	local_dir.z = -data->view_port.focal_length;
	ray.origin = rv.cam_origin;
	ray.dir = mat4_mult_vec3(&rv.cam_m, local_dir, 0.0);
	ray.dir = vec_normalize(ray.dir);
	return (ray);
}

t_vec3	vec_random_in_unit_sphere(void)
{
	t_vec3	p;

	while (1)
	{
		p.x = rand_double() * 2.0 - 1.0;
		p.y = rand_double() * 2.0 - 1.0;
		p.z = rand_double() * 2.0 - 1.0;
		if (vec_dot_scal(p, p) < 1.0)
			return (p);
	}
}

t_vec3	random_hemisphere_dir(t_vec3 normal)
{
	t_vec3	dir;

	dir = vec_random_in_unit_sphere();
	if (vec_dot_scal(dir, normal) < 0.0)
		dir = vec_scale(dir, -1.0);
	return (vec_normalize(dir));
}

void	set_hit_col(t_hit_r *rec, t_data *data)
{
	t_vec3	base_color;
	t_vec3	col_a;
	t_vec3	col_b;

	col_a.x = rec->obj_ptr->color.r / 255.0;
	col_a.y = rec->obj_ptr->color.g / 255.0;
	col_a.z = rec->obj_ptr->color.b / 255.0;
	if (rec->obj_ptr->has_texture && rec->obj_ptr->tex)
		rec->color = get_texture_color(rec->obj_ptr->tex, rec->u, rec->v);
	if (rec->obj_ptr->bump && rec->obj_ptr->has_bump)
		apply_bump(rec, rec->obj_ptr->bump, 10);
	if (data->has_checker)
	{
		col_b.x = data->checker_color.r / 255.0;
		col_b.y = data->checker_color.g / 255.0;
		col_b.z = data->checker_color.b / 255.0;
		base_color = get_checker_color(rec->u, rec->v, col_a, col_b);
		rec->color.r = base_color.x * 255.0;
		rec->color.g = base_color.y * 255.0;
		rec->color.b = base_color.z * 255.0;
	}
}

void	debug_bvh(t_vec3 *color_acc, t_data *data, t_hit_r *rec, t_ray ray)
{
	int			k;
	double		box_t;
	t_bvh_node	*node;
	bool		should_draw;

	if (data->debug && data->bvh_nodes)
	{
		k = 0;
		while (k < data->nodes_used)
		{
			node = &data->bvh_nodes[k];
			should_draw = (node->depth <= data->debug_depth);
			if (should_draw)
			{
				box_t = hit_aabb_edge(ray, node->box);
				if (box_t < DBL_MAX && box_t < rec->t)
				{
					*color_acc = node->debug_color;
					rec->t = box_t;
				}
			}
			k++;
		}
	}
}

t_vec3	reflect(int deph, t_data *data, t_hit_r *rec, t_ray ray)
{
	t_ro_re	utils;

	utils.r = rec->obj_ptr->reflectivity;
	utils.perfect_reflect = vec_sub(ray.dir,
		vec_scale(rec->normal, 2.0 * vec_dot_scal(ray.dir, rec->normal)));
	utils.fuzz = vec_scale(vec_random_in_unit_sphere(), rec->obj_ptr->rought);
	utils.spec_ray.dir = vec_normalize(vec_add(utils.perfect_reflect, utils.fuzz));
	utils.spec_ray.origin = vec_add(rec->p, vec_scale(rec->normal, EPSILON));
	utils.spec_color = check_hit(data, utils.spec_ray, deph - 1);
	utils.final_spec = vec_scale(utils.spec_color, utils.r);
	return (utils.final_spec);
}

t_vec3	rought_reflect(t_data *data, t_hit_r *rec, t_ray ray, int deph)
{
	t_ro_re	utils;

	ft_bzero(&utils.final_spec, sizeof(t_vec3));
	ft_bzero(&utils.final_diff, sizeof(t_vec3));
	utils.r = rec->obj_ptr->reflectivity;
	if (utils.r > 0.0) 
		utils.final_spec = reflect(deph, data, rec, ray);
	if (utils.r < 1.0 && data->diff_ok)
	{
		utils.diff_ray.dir = random_hemisphere_dir(rec->normal);
		utils.diff_ray.origin = vec_add(rec->p, vec_scale(rec->normal, EPSILON));
		utils.diff_color = check_hit(data, utils.diff_ray, deph - 1);
		utils.obj_col.x = rec->color.r / 255.0;
		utils.obj_col.y = rec->color.g / 255.0;
		utils.obj_col.z = rec->color.b / 255.0;
		utils.final_diff = vec_scale(vec_mult(utils.obj_col, utils.diff_color),
			(1.0 - utils.r) * 0.5);
	}
	return (vec_add(utils.final_spec, utils.final_diff));
}

t_vec3	tint_color_acc(double fresnel,
	t_vec3 refract_color, t_vec3 reflect_color, t_hit_r *rec)
{
	t_vec3	tint;
	t_vec3	tinted_refract;
	t_vec3	glass_tint;

	tint.x = rec->obj_ptr->color.r / 255.0;
	tint.y = rec->obj_ptr->color.g / 255.0;
	tint.z = rec->obj_ptr->color.b / 255.0;
	tinted_refract.x = refract_color.x * tint.x;
	tinted_refract.y = refract_color.y * tint.y;
	tinted_refract.z = refract_color.z * tint.z;
	glass_tint.x = tinted_refract.x * (1.0 - fresnel) + reflect_color.x * fresnel;
	glass_tint.y = tinted_refract.y * (1.0 - fresnel) + reflect_color.y * fresnel;
	glass_tint.z = tinted_refract.z * (1.0 - fresnel) + reflect_color.z * fresnel;
	return (glass_tint);
}

void	light_hit_t(t_light_hit *l_h, t_hit_r *rec)
{
	l_h->light_dir = vec_normalize(vec_sub(l_h->light->origin, rec->p));
	l_h->diff_strength = vec_dot_scal(rec->normal, l_h->light_dir);
	if (l_h->diff_strength > 0)
	{
		final_diffuse(&l_h->lights, l_h->light, l_h->diff_strength, rec->obj_ptr->kd);
		l_h->diffuse_total = vec_add(l_h->diffuse_total, l_h->lights.diffuse);
		l_h->minus_l = vec_scale(l_h->light_dir, -1.0);
		l_h->reflect_dir = vec_normalize(vec_sub(l_h->minus_l, vec_scale(rec->normal, 2.0 * vec_dot_scal(l_h->minus_l, rec->normal))));
		l_h->spec_factor = vec_dot_scal(l_h->view_dir, l_h->reflect_dir);
		if (l_h->spec_factor > 0)
		{
			l_h->spec_factor = pow(l_h->spec_factor, rec->obj_ptr->ns);
			l_h->intensity = l_h->spec_factor * l_h->light->ratio;
			l_h->scaled_light = vec_scale(l_h->l_col, l_h->intensity);
			l_h->final_spec = vec_mult(l_h->scaled_light, rec->obj_ptr->ks);
			l_h->specular_total = vec_add(l_h->specular_total, l_h->final_spec);
		}
	}
}

void	light_hit(t_hit_r *rec, t_data *data, t_vec3 *color_acc, t_ray ray)
{
	t_light_hit	l_h;

	l_h.light = data->light;
	l_h.diffuse_total = (t_vec3){0, 0, 0};
	l_h.specular_total = (t_vec3){0, 0, 0};
	calc_lights(&l_h.lights, *rec, data);
	*color_acc = l_h.lights.ambient;
	l_h.view_dir = vec_normalize(vec_scale(ray.dir, -1.0));
	l_h.light = data->light;
	while (l_h.light)
	{
		l_h.l_col.x = l_h.light->color.r / 255.0;
		l_h.l_col.y = l_h.light->color.g / 255.0;
		l_h.l_col.z = l_h.light->color.b / 255.0;
		l_h.lights.diffuse = l_h.l_col;
		if (is_in_shadow(data, rec, l_h.light) == false)
			light_hit_t(&l_h, rec);
		l_h.light = l_h.light->next;
	}
	*color_acc = vec_add(*color_acc, vec_add(l_h.diffuse_total, l_h.specular_total));
}

void	do_opacity_refract2(t_op_ni	*op_ni, t_hit_r *rec, t_ray ray)
{
	op_ni->re_col = (t_vec3){0, 0, 0};
	op_ni->rea_col = (t_vec3){0, 0, 0};
	op_ni->cos_theta_i = vec_dot_scal(ray.dir, rec->normal);
	if (op_ni->cos_theta_i < 0.0)
	{
		op_ni->is_inside = false;
		op_ni->out_normal = rec->normal;
		op_ni->cos_theta_i = -op_ni->cos_theta_i;
		op_ni->eta_i = 1.0;
		op_ni->eta_t = rec->obj_ptr->ni;
	}
	else
	{
		op_ni->is_inside = true;
		op_ni->out_normal = vec_scale(rec->normal, -1.0);
		op_ni->eta_i = rec->obj_ptr->ni;
		op_ni->eta_t = 1.0;
	}
	op_ni->eta_ratio = op_ni->eta_i / op_ni->eta_t;
	op_ni->r0 = (op_ni->eta_i - op_ni->eta_t) / (op_ni->eta_i + op_ni->eta_t);
	op_ni->r0 = op_ni->r0 * op_ni->r0;
}

t_vec3	do_opacity_refract(t_data *data, t_hit_r *rec, int deph, t_ray ray)
{
	t_op_ni	op_ni;

	do_opacity_refract2(&op_ni, rec, ray);
	op_ni.fresnel = op_ni.r0 + (1.0 - op_ni.r0) * pow(1.0 - op_ni.cos_theta_i, 5.0);
	op_ni.re_ray.dir = vec_normalize(vec_sub(ray.dir,
				vec_scale(op_ni.out_normal, 2.0 * vec_dot_scal(ray.dir, op_ni.out_normal))));
	op_ni.re_ray.origin = vec_add(rec->p, vec_scale(op_ni.out_normal, EPSILON));
	op_ni.re_col = check_hit(data, op_ni.re_ray, deph - 1);
	op_ni.sin2_theta_t = op_ni.eta_ratio * op_ni.eta_ratio * (1.0 - op_ni.cos_theta_i * op_ni.cos_theta_i);
	if (op_ni.sin2_theta_t > 1.0)
		op_ni.fresnel = 1.0;
	else
	{
		op_ni.cos_theta_t = sqrt(1.0 - op_ni.sin2_theta_t);
		op_ni.r_dir_part1 = vec_scale(ray.dir, op_ni.eta_ratio);
		op_ni.r_dir_part2 = vec_scale(op_ni.out_normal,
				op_ni.eta_ratio * op_ni.cos_theta_i - op_ni.cos_theta_t);
		op_ni.rea_ray.dir = vec_normalize(vec_add(op_ni.r_dir_part1, op_ni.r_dir_part2));
		op_ni.rea_ray.origin = vec_sub(rec->p, vec_scale(op_ni.out_normal, EPSILON));
		op_ni.re_col = check_hit(data, op_ni.rea_ray, deph - 1);
	}
	return (tint_color_acc(op_ni.fresnel, op_ni.rea_col,
		op_ni.re_col, rec));
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

void	do_samples(t_data *data, t_idxs idxs, t_render_v rv, t_vec3 *color_acc)
{
	t_ray	ray;
	t_vec3	sample_color;

	idxs.s = 0;
	while (idxs.s < data->s_per_pixs)
	{
		if (data->s_per_pixs == 1)
		{
			rv.u = ((double)idxs.x + 0.5) * rv.inv_width;
			rv.v = ((double)idxs.y + 0.5) * rv.inv_height;
		}
		else
		{
			rv.u = ((double)idxs.x + rand_double()) * rv.inv_width;
			rv.v = ((double)idxs.y + rand_double()) * rv.inv_height;
		}
		ray = calc_ray(rv, data);
		sample_color = check_hit(data, ray, data->deph);
		*color_acc = vec_add(*color_acc, sample_color);
		idxs.s++;
	}
}

void	draw_step(t_ic ic, t_data *data, t_idxs idxs, t_thread_info *info)
{
	int			idx;
	int			y1;
	int			x1;
	t_vec2int	vec;

	vec.y = 0;
	while (vec.y < data->step)
	{
		vec.x = 0;
		y1 = idxs.y + vec.y;
		if (y1 >= info->end_y)
			break ;
		while (vec.x < data->step)
		{
			x1 = idxs.x + vec.x;
			if (x1 >= info->end_x)
				break ;
			idx = (y1) * data->width + (x1);
			data->pixels[idx].rgba = (ic.ir << 24)
				| (ic.ig << 16) | (ic.ib << 8) | 0xFF;
			vec.x++;
		}
		vec.y++;
	}
}

void	set_final_color(t_vec3 color_acc,
	t_data *data, t_idxs idxs, t_thread_info *info)
{
	t_vec3		final_color;
	t_ic		ic;

	final_color = vec_scale(color_acc, 1.0 / data->s_per_pixs);
	if (final_color.x > 1.0)
		final_color.x = 1.0;
	if (final_color.y > 1.0)
		final_color.y = 1.0;
	if (final_color.z > 1.0)
		final_color.z = 1.0;
	final_color.x = pow(final_color.x, 0.45454545454);
	final_color.y = pow(final_color.y, 0.45454545454);
	final_color.z = pow(final_color.z, 0.45454545454);
	ic.ir = (int)(255.999 * final_color.x);
	ic.ig = (int)(255.999 * final_color.y);
	ic.ib = (int)(255.999 * final_color.z);
	draw_step(ic, data, idxs, info);
}

void	add_finish(t_data *data)
{
	pthread_mutex_lock(&data->finish_count);
	data->finish++;
	pthread_mutex_unlock(&data->finish_count);
}

/*
	formule fov equation
	Height = 2×tan(2/FOV​) × FocalLength

	Couleur Amb ​= Couleur Obj ​× (Couleur Amb​ × Ratio Amb​)
	Couleur Diff​ = Couleur Obj ​× (Couleur Light ​× Ratio Light​)
		× (Normal ⋅ Light Dir)

	couleur spec ​=C light​⋅ks​⋅(max(0,V⋅R)) exposant α
*/
void	render(void *arg)
{
	t_thread_info	*info;
	t_data			*data;
	t_vec3			color_acc;
	t_render_v		rv;

	info = (t_thread_info *)arg;
	data = info->data;
	rv.inv_width = 1.0 / (data->width - 1);
	rv.inv_height = 1.0 / (data->height - 1);
	rv.cam_m = look_at(data->cam.origin, data->cam.dir, data->cam.up_guide);
	rv.cam_origin = mat4_mult_vec3(&rv.cam_m, (t_vec3){0, 0, 0}, 1.0);
	rv.idxs.y = info->start_y;
	while (rv.idxs.y < info->end_y)
	{
		rv.idxs.x = info->start_x;
		while (rv.idxs.x < info->end_x)
		{
			color_acc = (t_vec3){0, 0, 0};
			do_samples(data, rv.idxs, rv, &color_acc);
			set_final_color(color_acc, data, rv.idxs, info);
			rv.idxs.x += data->step;
		}
		rv.idxs.y += data->step;
	}
	add_finish(data);
}
