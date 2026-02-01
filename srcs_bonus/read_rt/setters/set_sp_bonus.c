/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_sp_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:39 by titan             #+#    #+#             */
/*   Updated: 2026/02/01 13:55:55 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_sp(t_data *data, char *line, int i)
{
	t_obj	*new_sp;
	t_mat_t	t;

	check_missing_info(data, line, i);
	t.center = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	t.diameter = rt_atod(&line);
	check_positive(data, t.diameter, i);
	check_missing_info(data, line, i);
	t.col = parse_color(&line, data, i);
	check_extra_info(data, line, i);
	new_sp = malloc(sizeof(t_obj));
	if (!new_sp)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_sp->type = CALC_SP;
	new_sp->color = t.col;
	mat4_initial(&t.scale);
	mat4_initial(&t.trans);
	mat4_scal(&t.scale, (t_vec3){t.diameter, t.diameter, t.diameter});
	mat4_translation(&t.trans, t.center);
	t.final = mat4_mult(&t.trans, &t.scale);
	new_sp->transform = t.final;
	new_sp->inverse_transform = mat4_inverse(&t.final);
	new_sp->next = NULL;
	ft_objadd_back(&data->objs, new_sp);
}
