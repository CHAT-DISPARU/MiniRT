/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_sq_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:06:23 by titan             #+#    #+#             */
/*   Updated: 2026/02/13 15:56:42 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	read_sq(t_data *data, char **line, int i, t_mat_t *t)
{
	check_missing_info(data, *line, i);
	t->center = parse_vec3(line, data, i);
	check_missing_info(data, *line, i);
	t->rot_vec = parse_vec3(line, data, i);
	check_norm_vec(data, t->rot_vec, i);
	check_missing_info(data, *line, i);
	t->width = rt_atod(line);
	check_positive(data, t->width, i);
	check_missing_info(data, *line, i);
	t->height = rt_atod(line);
	check_positive(data, t->height, i);
	check_missing_info(data, *line, i);
	t->col = parse_color(line, data, i);
	t->reflectivity = parse_reflectivity(line);
	t->rought = parse_roughness(line);
	check_extra_info(data, *line, i);
}

void	set_sq(t_data *data, char *line, int i)
{
	t_obj		*new_sq;
	t_mat_t		t;

	read_sq(data, &line, i, &t);
	new_sq = ft_calloc(1, sizeof(t_obj));
	if (!new_sq)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_sq->type = CALC_SQ;
	new_sq->color = t.col;
	mat4_initial(&t.trans);
	mat4_initial(&t.scale);
	mat4_scal(&t.scale, (t_vec3){t.width / 2.0, 1.0, t.height / 2.0});
	t.rot = mat4_align_vectors((t_vec3){0, 1, 0}, vec_normalize(t.rot_vec));
	mat4_translation(&t.trans, t.center);
	t.final = mat4_mult(&t.rot, &t.scale);
	t.final = mat4_mult(&t.trans, &t.final);
	new_sq->transform = t.final;
	new_sq->inverse_transform = mat4_inverse(&t.final);
	new_sq->reflectivity = t.reflectivity;
	new_sq->rought = t.rought;
	char	*path = get_texture_path(&line);
	if (path)
	{
		new_sq->has_texture = true;
		new_sq->tex  = load_texture(data, path);
	}
	else
		new_sq->has_texture = false;
	new_sq->next = NULL;
	ft_objadd_back(&data->objs, new_sq);
}
