/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_hy_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:06:16 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/02 21:25:31 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	read_hy(t_data *data, char **line, int i, t_mat_t *t)
{
	check_missing_info(data, *line, i);
	t->center = parse_vec3(line, data, i);
	check_missing_info(data, *line, i);
	t->rot_vec = parse_vec3(line, data, i);
	check_norm_vec(data, t->rot_vec, i);
	check_missing_info(data, *line, i);
	t->diameter = rt_atod(line);
	check_positive(data, t->diameter, i);
	check_missing_info(data, *line, i);
	t->diameter2 = rt_atod(line);
	check_positive(data, t->diameter2, i);
	check_missing_info(data, *line, i);
	t->height = rt_atod(line);
	check_positive(data, t->height, i);
	check_missing_info(data, *line, i);
	t->col = parse_color(line, data, i);
	t->reflectivity = parse_reflectivity(line);
	t->rought = parse_roughness(line);
}

void	set_hy(t_data *data, char *line, int i)
{
	t_obj	*new_hy;
	t_mat_t	t;

	read_hy(data, &line, i, &t);
	new_hy = ft_calloc(1, sizeof(t_obj));
	if (!new_hy)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_hy->type = CALC_HY;
	new_hy->color = t.col;
	mat4_initial(&t.trans);
	mat4_initial(&t.scale);
	mat4_scal(&t.scale,
		(t_vec3){t.height / 2.0, t.diameter / 2.0, t.diameter2 / 2.0});
	mat4_translation(&t.trans, t.center);
	t.rot = mat4_align_vectors((t_vec3){0, 0, 1}, vec_normalize(t.rot_vec));
	t.final = t.rot;
	t.final = mat4_mult(&t.trans, &t.final);
	new_hy->rad_1 = t.diameter / 2.0;
	new_hy->rad_2 = t.diameter2 / 2.0;
	new_hy->height = t.height;
	new_hy->transform = t.final;
	new_hy->inverse_transform = mat4_inverse(&t.final);
	new_hy->reflectivity = t.reflectivity;
	new_hy->rought = t.rought;
	new_hy->ka = (t_vec3){0.2, 0.2, 0.2};
	new_hy->ks = (t_vec3){1, 1, 1};
	new_hy->kd = (t_vec3){0.8, 0.8, 0.8};
	new_hy->ns = 32;
	new_hy->opacity = 1;
	char	*path = get_texture_path(&line);
	char	*path2 = get_texture_path(&line);
	check_extra_info(data, line, i);
	if (path)
	{
		new_hy->has_texture = true;
		new_hy->tex = load_texture(data, path, NULL);
	}
	else
		new_hy->has_texture = false;
	if (path2)
	{
		new_hy->has_bump = true;
		new_hy->bump = load_texture(data, path2, NULL);
	}
	else
		new_hy->has_bump = false;
	new_hy->next = NULL;
	ft_objadd_back(&data->objs, new_hy);
}
