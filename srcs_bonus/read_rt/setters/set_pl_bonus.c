/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pl_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:44 by titan             #+#    #+#             */
/*   Updated: 2026/02/01 13:55:55 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_pl(t_data *data, char *line, int i)
{
	t_obj	*new_pl;
	t_mat_t	t;

	check_missing_info(data, line, i);
	t.center = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	t.rot_vec = parse_vec3(&line, data, i);
	check_norm_vec(data, t.rot_vec, i);
	check_missing_info(data, line, i);
	t.col = parse_color(&line, data, i);
	check_extra_info(data, line, i);
	new_pl = malloc(sizeof(t_obj));
	if (!new_pl)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_pl->type = CALC_PL;
	new_pl->color = t.col;
	mat4_initial(&t.trans);
	mat4_translation(&t.trans, t.center);
	t.rot = mat4_align_vectors((t_vec3){0, 1, 0}, vec_normalize(t.rot_vec));
	t.final = mat4_mult(&t.trans, &t.rot);
	new_pl->transform = t.final;
	new_pl->inverse_transform = mat4_inverse(&t.final);
	new_pl->next = NULL;
	ft_objadd_back(&data->objs, new_pl);
}
