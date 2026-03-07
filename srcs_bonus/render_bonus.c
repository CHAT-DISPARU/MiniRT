/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:57:53 by titan             #+#    #+#             */
/*   Updated: 2026/03/07 13:05:52 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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

/*
	rebond
	R=I−2(I⋅N)N
*/
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

t_vec3	check_hit(t_data *data, t_ray ray, int deph)
{
	t_color_c	lights;
	t_vec3		color_acc = {0, 0, 0};
	t_vec3		diffuse_total = {0, 0, 0};
	t_vec3		specular_total = {0, 0, 0};
	t_hit_r		rec;
	t_light		*light;
	t_vec3		light_dir;
	t_vec3		view_dir;
	t_vec3		reflect_dir;
	t_vec3		minus_l;
	t_vec3		l_col;
	double		diff_strength;
	double		spec_factor;

	if (deph <= 0)
		return (color_acc);
	light = data->light;
	ft_bzero(&rec, sizeof(t_hit_r));
	rec.t = DBL_MAX;
	if (hit_someting(data, ray, &rec))
	{
		t_vec3	base_color;
		t_vec3	col_a;
		t_vec3	col_b;
		col_a.x = rec.obj_ptr->color.r / 255.0;
		col_a.y = rec.obj_ptr->color.g / 255.0;
		col_a.z = rec.obj_ptr->color.b / 255.0;
		if (rec.obj_ptr->has_texture && rec.obj_ptr->tex)
			rec.color = get_texture_color(rec.obj_ptr->tex, rec.u, rec.v);
		if (rec.obj_ptr->bump && rec.obj_ptr->has_bump)
			apply_bump(&rec, rec.obj_ptr->bump, 10);
		if (data->has_checker)
		{
			col_b.x = data->checker_color.r / 255.0;
			col_b.y = data->checker_color.g / 255.0;
			col_b.z = data->checker_color.b / 255.0;
			base_color = get_checker_color(rec.u, rec.v, col_a, col_b);
			rec.color.r = base_color.x * 255.0;
			rec.color.g = base_color.y * 255.0;
			rec.color.b = base_color.z * 255.0;
		}
		calc_lights(&lights, rec, data, light);
		color_acc = lights.ambient;
		view_dir = vec_normalize(vec_scale(ray.dir, -1.0));
		while (light)
		{
			l_col.x = light->color.r / 255.0;
			l_col.y = light->color.g / 255.0;
			l_col.z = light->color.b / 255.0;
			lights.diffuse = l_col;
			if (is_in_shadow(data, &rec, light) == false)
			{
				light_dir = vec_normalize(vec_sub(light->origin, rec.p));
				diff_strength = vec_dot_scal(rec.normal, light_dir);
				if (diff_strength > 0)
				{
					final_diffuse(&lights, light, diff_strength, rec.obj_ptr->kd);
					diffuse_total = vec_add(diffuse_total, lights.diffuse);
					minus_l = vec_scale(light_dir, -1.0);
					reflect_dir = vec_normalize(vec_sub(minus_l, vec_scale(rec.normal, 2.0 * vec_dot_scal(minus_l, rec.normal))));
					spec_factor = vec_dot_scal(view_dir, reflect_dir);
					if (spec_factor > 0)
					{
						spec_factor = pow(spec_factor, rec.obj_ptr->ns);
						double intensity = spec_factor * light->ratio;
						t_vec3	scaled_light = vec_scale(l_col, intensity);
						t_vec3	final_spec = vec_mult(scaled_light, rec.obj_ptr->ks);
						specular_total = vec_add(specular_total, final_spec);
					}
				}
			}
			light = light->next;
		}
		color_acc = vec_add(color_acc, vec_add(diffuse_total, specular_total));
		if (deph > 1)
		{
			t_vec3	perfect_reflect;
			t_vec3	fuzz;
			t_vec3	spec_dir;
			t_vec3	diff_dir;
			t_ray	spec_ray;
			t_ray	diff_ray;
			t_vec3	spec_color;
			t_vec3	diff_color;
			t_vec3	obj_col;
			t_vec3	final_spec = {0, 0, 0};
			t_vec3	final_diff = {0, 0, 0};
			double	r = rec.obj_ptr->reflectivity;

			if (rec.obj_ptr->opacity < 1)
			{
				t_vec3	out_normal;
				t_ray	reflect_ray;
				t_ray	refract_ray;
				t_vec3	reflect_color = {0, 0, 0};
				t_vec3	refract_color = {0, 0, 0};
				t_vec3	r_dir_part1;
				t_vec3	r_dir_part2;
				double	eta_i;
				double	eta_t;
				double	eta_ratio;
				double	cos_theta_i;
				double	sin2_theta_t;
				double	cos_theta_t;
				double	r0;
				double	fresnel;
				bool	is_inside;

				cos_theta_i = vec_dot_scal(ray.dir, rec.normal);
				if (cos_theta_i < 0.0)
				{
					is_inside = false;
					out_normal = rec.normal;
					cos_theta_i = -cos_theta_i;
					eta_i = 1.0;
					eta_t = rec.obj_ptr->ni;
				}
				else
				{
					is_inside = true;
					out_normal = vec_scale(rec.normal, -1.0);
					eta_i = rec.obj_ptr->ni;
					eta_t = 1.0;
				}
				eta_ratio = eta_i / eta_t;
				r0 = (eta_i - eta_t) / (eta_i + eta_t);
				r0 = r0 * r0;
				fresnel = r0 + (1.0 - r0) * pow(1.0 - cos_theta_i, 5.0);
				reflect_ray.dir = vec_normalize(vec_sub(ray.dir,
							vec_scale(out_normal, 2.0 * vec_dot_scal(ray.dir, out_normal))));
				reflect_ray.origin = vec_add(rec.p, vec_scale(out_normal, EPSILON));
				reflect_color = check_hit(data, reflect_ray, deph - 1);
				sin2_theta_t = eta_ratio * eta_ratio * (1.0 - cos_theta_i * cos_theta_i);
				if (sin2_theta_t > 1.0)
					fresnel = 1.0;
				else
				{
					cos_theta_t = sqrt(1.0 - sin2_theta_t);
					r_dir_part1 = vec_scale(ray.dir, eta_ratio);
					r_dir_part2 = vec_scale(out_normal, eta_ratio * cos_theta_i - cos_theta_t);
					refract_ray.dir = vec_normalize(vec_add(r_dir_part1, r_dir_part2));
					refract_ray.origin = vec_sub(rec.p, vec_scale(out_normal, EPSILON));
					refract_color = check_hit(data, refract_ray, deph - 1);
				}
				t_vec3  tint;
				tint.x = rec.obj_ptr->color.r / 255.0;
				tint.y = rec.obj_ptr->color.g / 255.0;
				tint.z = rec.obj_ptr->color.b / 255.0;
				t_vec3  tinted_refract;
				t_vec3  glass_tint;
				tinted_refract.x = refract_color.x * tint.x;
				tinted_refract.y = refract_color.y * tint.y;
				tinted_refract.z = refract_color.z * tint.z;
				glass_tint.x = tinted_refract.x * (1.0 - fresnel) + reflect_color.x * fresnel;
				glass_tint.y = tinted_refract.y * (1.0 - fresnel) + reflect_color.y * fresnel;
				glass_tint.z = tinted_refract.z * (1.0 - fresnel) + reflect_color.z * fresnel;
				color_acc = vec_add(glass_tint, color_acc);
			}
			else
			{
				if (r > 0.0) 
				{
					r = rec.obj_ptr->reflectivity;
					perfect_reflect = vec_sub(ray.dir, vec_scale(rec.normal, 2.0 * vec_dot_scal(ray.dir, rec.normal)));
					fuzz = vec_scale(vec_random_in_unit_sphere(), rec.obj_ptr->rought);
					spec_dir = vec_normalize(vec_add(perfect_reflect, fuzz));
					spec_ray.origin = vec_add(rec.p, vec_scale(rec.normal, EPSILON));
					spec_ray.dir = spec_dir;
					spec_color = check_hit(data, spec_ray, deph - 1);
					final_spec = vec_scale(spec_color, r);
				}
				if (r < 1.0 && data->diff_ok)
				{
					diff_dir = random_hemisphere_dir(rec.normal);
					diff_ray.origin = vec_add(rec.p, vec_scale(rec.normal, EPSILON));
					diff_ray.dir = diff_dir;
					diff_color = check_hit(data, diff_ray, deph - 1);
					obj_col.x = rec.color.r / 255.0;
					obj_col.y = rec.color.g / 255.0;
					obj_col.z = rec.color.b / 255.0;
					final_diff = vec_scale(vec_mult(obj_col, diff_color), (1.0 - r) * 0.5);
				}
				color_acc = vec_add(color_acc, vec_add(final_spec, final_diff));
			}
		}
	}
	
	if (data->debug && data->bvh_nodes)
	{
		int			k = 0;
		double		box_t;
		t_bvh_node	*node;
		bool		should_draw;

		while (k < data->nodes_used)
		{
			node = &data->bvh_nodes[k];
			should_draw = (node->depth <= data->debug_depth);
			if (should_draw)
			{
				box_t = hit_aabb_edge(ray, node->box);
				if (box_t < DBL_MAX && box_t < rec.t)
				{
					color_acc = node->debug_color;
					rec.t = box_t;
				}
			}
			k++;
		}
	}
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

void	set_final_color(t_vec3 color_acc, t_data *data, t_idxs idxs, t_thread_info *info)
{
	int			idx;
	int			x;
	int			y;
	int			y1;
	int			x1;
	t_vec3		final_color;
	t_ic		ic;

	y = idxs.y;
	x = idxs.x;
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
	y = 0;
	while (y < data->step)
	{
		x = 0;
		y1 = idxs.y + y;
		if (y1 >= info->end_y)
			break ;
		while (x < data->step)
		{
			x1 = idxs.x + x;
			if (x1 >= info->end_x)
				break ;
			idx = (y1) * data->width + (x1);
			data->pixels[idx].rgba = (ic.ir << 24)
				| (ic.ig << 16) | (ic.ib << 8) | 0xFF;
			x++;
		}
		y++;
	}

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
	t_thread_info	*info = (t_thread_info *)arg;
	t_data			*data = info->data;
	t_idxs		idxs;
	t_vec3		color_acc;
	t_render_v	rv;
	int             total_lines;

	rv.inv_width = 1.0 / (data->width - 1);
	rv.inv_height = 1.0 / (data->height - 1);
	rv.cam_m = look_at(data->cam.origin, data->cam.dir, data->cam.up_guide);
	rv.cam_origin = mat4_mult_vec3(&rv.cam_m, (t_vec3){0, 0, 0}, 1.0);
	total_lines = (info->end_y - info->start_y) / data->step;
	if (total_lines <= 0)
		total_lines = 1;
	idxs.y = info->start_y;;
	while (idxs.y < info->end_y)
	{
		idxs.x = info->start_x;
		while (idxs.x < info->end_x)
		{
			color_acc = (t_vec3){0, 0, 0};
			do_samples(data, idxs, rv, &color_acc);
			set_final_color(color_acc, data, idxs, info);
			idxs.x += data->step;
		}
		idxs.y += data->step;
	}
	pthread_mutex_lock(&data->finish_count);
	data->finish++;
	pthread_mutex_unlock(&data->finish_count);
}
