/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cy_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:55 by titan             #+#    #+#             */
/*   Updated: 2026/02/01 13:55:55 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	read_cy(t_data *data, char **line, int i, t_mat_t *t)
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
	t->height = rt_atod(line);
	check_positive(data, t->height, i);
	check_missing_info(data, *line, i);
	t->col = parse_color(line, data, i);
	check_extra_info(data, *line, i);
}

void	set_cy(t_data *data, char *line, int i)
{
	t_obj	*new_cy;
	t_mat_t	t;

	read_cy(data, &line, i, &t);
	new_cy = malloc(sizeof(t_obj));
	if (!new_cy)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_cy->type = CALC_CY;
	new_cy->color = t.col;
	mat4_initial(&t.trans);
	mat4_initial(&t.scale);
	mat4_scal(&t.scale,
		(t_vec3){t.height / 2.0, t.diameter / 2.0, t.diameter / 2.0});
	mat4_translation(&t.trans, t.center);
	t.rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(t.rot_vec));
	t.final = mat4_mult(&t.rot, &t.scale);
	t.final = mat4_mult(&t.trans, &t.final);
	new_cy->transform = t.final;
	new_cy->inverse_transform = mat4_inverse(&t.final);
	new_cy->next = NULL;
	ft_objadd_back(&data->objs, new_cy);
}
