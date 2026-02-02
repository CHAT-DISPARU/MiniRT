/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tr_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 14:38:20 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/02 15:19:21 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>


void	read_tr(t_data *data, char **line, int i, t_mat_t *t)
{
	check_missing_info(data, *line, i);
	t->col = parse_color(line, data, i);
	check_extra_info(data, *line, i);
}

void	set_tr(t_data *data, char *line, int i)
{
	t_obj		*new_tr;
	t_mat_t		t;

	check_missing_info(data, line, i);
	t.tri.p1 = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	t.tri.p2 = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	t.tri.p3 = parse_vec3(&line, data, i);
	read_tr(data, &line, i, &t);
	new_tr = malloc(sizeof(t_obj));
	if (!new_tr)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_tr->tri = t.tri;
	new_tr->type = CALC_TR;
	new_tr->color = t.col;
	new_tr->next = NULL;
	ft_objadd_back(&data->objs, new_tr);
}
