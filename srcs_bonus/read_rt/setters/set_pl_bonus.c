/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pl_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:44 by titan             #+#    #+#             */
/*   Updated: 2026/03/09 12:35:01 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	read_pl(t_data *data, char **line, int i, t_mat_t *t)
{
	check_missing_info(data, *line, i);
	t->center = parse_vec3(line, data, i);
	check_missing_info(data, *line, i);
	t->rot_vec = parse_vec3(line, data, i);
	check_norm_vec(data, t->rot_vec, i);
	check_missing_info(data, *line, i);
	t->col = parse_color(line, data, i);
	t->reflectivity = parse_reflectivity(line);
	t->rought = parse_roughness(line);
	skip_spaces(line);
	t->opacity = 1.0;
	t->ni = 1.0;
	if (**line && !is_space(**line) && **line != '\n' && **line != '\0' && ft_isdigit(**line))
		t->opacity = rt_atod(line);
	skip_spaces(line);
	if (**line && !is_space(**line) && **line != '\n' && **line != '\0' && ft_isdigit(**line))
		t->ni = rt_atod(line);
}

void	set_obj_lum(t_obj **new)
{
	(*new)->ka = (t_vec3){0.2, 0.2, 0.2};
	(*new)->ks = (t_vec3){1, 1, 1};
	(*new)->kd = (t_vec3){0.8, 0.8, 0.8};
	(*new)->ns = 32;
}

void	set_pl(t_data *data, char *line, int i)
{
	t_obj	*new_pl;
	t_mat_t	t;

	read_pl(data, &line, i, &t);
	new_pl = ft_calloc(1, sizeof(t_obj));
	if (!new_pl)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_pl->type = CALC_PL;
	new_pl->color = t.col;
	mat4_initial(&t.trans);
	mat4_translation(&t.trans, t.center);
	t.rot = mat4_align_vectors((t_vec3){0, 1, 0}, vec_normalize(t.rot_vec));
	t.final = mat4_mult(&t.trans, &t.rot);
	new_pl->transform = t.final;
	new_pl->reflectivity = t.reflectivity;
	new_pl->inverse_transform = mat4_inverse(&t.final);
	new_pl->rought = t.rought;
	set_obj_lum(&new_pl);
	new_pl->opacity = t.opacity;
	ft_objadd_back(&data->objs, new_pl);
	new_pl->next = NULL;
	new_pl->ni = t.ni;
	set_tex(data, &line, i, new_pl);
}
