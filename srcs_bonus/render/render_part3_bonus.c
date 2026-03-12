/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_part3_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:06:13 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 11:06:39 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_vec3	reflect(int deph, t_data *data, t_hit_r *rec, t_ray ray)
{
	t_ro_re	utils;

	utils.r = rec->obj_ptr->reflectivity;
	utils.perfect_reflect = vec_sub(ray.dir,
			vec_scale(rec->normal, 2.0 * vec_dot_scal(ray.dir, rec->normal)));
	utils.fuzz = vec_scale(vec_random_in_unit_sphere(), rec->obj_ptr->rought);
	utils.spec_ray.dir
		= vec_normalize(vec_add(utils.perfect_reflect, utils.fuzz));
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
		utils.diff_ray.origin = vec_add(rec->p,
				vec_scale(rec->normal, EPSILON));
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
	glass_tint.x = tinted_refract.x * (1.0 - fresnel)
		+ reflect_color.x * fresnel;
	glass_tint.y = tinted_refract.y * (1.0 - fresnel)
		+ reflect_color.y * fresnel;
	glass_tint.z = tinted_refract.z * (1.0 - fresnel)
		+ reflect_color.z * fresnel;
	return (glass_tint);
}

void	light_hit_t(t_light_hit *l_h, t_hit_r *rec)
{
	l_h->light_dir = vec_normalize(vec_sub(l_h->light->origin, rec->p));
	l_h->diff_strength = vec_dot_scal(rec->normal, l_h->light_dir);
	if (l_h->diff_strength > 0)
	{
		final_diffuse(&l_h->lights, l_h->light,
			l_h->diff_strength, rec->obj_ptr->kd);
		l_h->diffuse_total = vec_add(l_h->diffuse_total, l_h->lights.diffuse);
		l_h->minus_l = vec_scale(l_h->light_dir, -1.0);
		l_h->reflect_dir = vec_normalize(vec_sub(l_h->minus_l,
					vec_scale(rec->normal, 2.0
						* vec_dot_scal(l_h->minus_l, rec->normal))));
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
	*color_acc = vec_add(*color_acc,
			vec_add(l_h.diffuse_total, l_h.specular_total));
}
