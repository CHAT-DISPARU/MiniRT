/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_co_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:25:02 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/04 13:21:25 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	read_co(t_data *data, char **line, int i, t_mat_t *t)
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

void	set_co(t_data *data, char *line, int i)
{
	t_obj	*new_co;
	t_mat_t	t;

	read_co(data, &line, i, &t);
	new_co = malloc(sizeof(t_obj));
	if (!new_co)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_co->type = CALC_CO;
	new_co->color = t.col;
	mat4_initial(&t.trans);
	mat4_translation(&t.trans, t.center);
	t.rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(t.rot_vec));
	t.final = t.rot;
	t.final = mat4_mult(&t.trans, &t.final);
	new_co->rad_1 = t.diameter;
	new_co->height = t.height;
	new_co->transform = t.final;
	new_co->inverse_transform = mat4_inverse(&t.final);
	new_co->next = NULL;
	ft_objadd_back(&data->objs, new_co);
}
