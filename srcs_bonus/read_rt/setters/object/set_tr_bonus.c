/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tr_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 14:38:20 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/10 13:10:03 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	read_tr(t_data *data, char **line, int i, t_mat_t *t)
{
	check_missing_info(data, *line, i);
	t->tri.p1 = parse_vec3(line, data, i);
	check_missing_info(data, *line, i);
	t->tri.p2 = parse_vec3(line, data, i);
	check_missing_info(data, *line, i);
	t->tri.p3 = parse_vec3(line, data, i);
	check_missing_info(data, *line, i);
	t->col = parse_color(line, data, i);
	t->reflectivity = parse_reflectivity(line);
	t->rought = parse_roughness(line);
	t->opacity = 1.0;
	t->ni = 1.0;
	skip_spaces(line);
	if (**line && !is_space(**line) && **line != '\n'
		&& **line != '\0' && ft_isdigit(**line))
		t->opacity = rt_atod(line);
	skip_spaces(line);
	if (**line && !is_space(**line) && **line != '\n'
		&& **line != '\0' && ft_isdigit(**line))
		t->ni = rt_atod(line);
}

void	set_tr(t_data *data, char *line, int i)
{
	t_obj		*new_tr;
	t_mat_t		t;
	t_vec3		edge1;
	t_vec3		edge2;

	read_tr(data, &line, i, &t);
	skip_spaces(&line);
	new_tr = ft_calloc(1, sizeof(t_obj));
	if (!new_tr)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_tr->tri = t.tri;
	edge1 = vec_sub(new_tr->tri.p2, new_tr->tri.p1);
	edge2 = vec_sub(new_tr->tri.p3, new_tr->tri.p1);
	new_tr->tri.normal = vec_normalize(vec_cross(edge1, edge2));
	new_tr->type = CALC_TR;
	new_tr->color = t.col;
	new_tr->reflectivity = t.reflectivity;
	set_obj_lum(&new_tr);
	new_tr->opacity = t.opacity;
	new_tr->ni = t.ni;
	new_tr->rought = t.rought;
	new_tr->next = NULL;
	new_tr->next = data->objs;
	data->objs = new_tr;
	set_tex(data, &line, i, new_tr);
}
