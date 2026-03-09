/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_sp_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:39 by titan             #+#    #+#             */
/*   Updated: 2026/03/09 12:37:20 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	read_sp(t_data *data, char **line, int i, t_mat_t *t)
{
	check_missing_info(data, *line, i);
	t->center = parse_vec3(line, data, i);
	check_missing_info(data, *line, i);
	t->diameter = rt_atod(line);
	check_positive(data, t->diameter, i);
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

void	calc_final_mat(t_mat_t *t)
{
	mat4_initial(&t->scale);
	mat4_initial(&t->trans);
	mat4_scal(&t->scale, (t_vec3){t->diameter, t->diameter, t->diameter});
	t->rot = mat4_align_vectors((t_vec3){0, 1, 0}, vec_normalize(t->rot_vec));
	mat4_translation(&t->trans, t->center);
	t->final = mat4_mult(&t->rot, &t->scale);
	t->final = mat4_mult(&t->trans, &t->final);
}

void	set_sp(t_data *data, char *line, int i)
{
	t_obj	*new_sp;
	t_mat_t	t;

	read_sp(data, &line, i, &t);
	new_sp = ft_calloc(1, sizeof(t_obj));
	if (!new_sp)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_sp->type = CALC_SP;
	new_sp->color = t.col;
	calc_final_mat(&t);
	new_sp->transform = t.final;
	new_sp->inverse_transform = mat4_inverse(&t.final);
	new_sp->reflectivity = t.reflectivity;
	new_sp->rought = t.rought;
	set_obj_lum(&new_sp);
	new_sp->opacity = t.opacity;
	new_sp->ni = t.ni;
	ft_objadd_back(&data->objs, new_sp);
	new_sp->next = NULL;
	set_tex(data, &line, i, new_sp);
}
