/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_co_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:25:02 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/07 13:01:26 by gajanvie         ###   ########.fr       */
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

void	set_tex(t_data *data, char **line, int i, t_obj *new)
{
	char	*path;
	char	*path2;

	path = get_texture_path(line);
	path2 = get_texture_path(line);
	check_extra_info(data, *line, i);
	if (path)
	{
		new->has_texture = true;
		new->tex = load_texture(data, path, NULL);
	}
	else
		new->has_texture = false;
	if (path2)
	{
		new->has_bump = true;
		new->bump = load_texture(data, path2, NULL);
	}
	else
		new->has_bump = false;
}

void	set_new_co(t_mat_t t, t_obj *new_co)
{
	new_co->rad_1 = t.diameter;
	new_co->height = t.height;
	new_co->transform = t.final;
	new_co->inverse_transform = mat4_inverse(&t.final);
	new_co->reflectivity = t.reflectivity;
	new_co->rought = t.rought;
	new_co->ka = (t_vec3){0.2, 0.2, 0.2};
	new_co->ks = (t_vec3){1, 1, 1};
	new_co->kd = (t_vec3){0.8, 0.8, 0.8};
	new_co->ns = 32;
	new_co->opacity = t.opacity;
	new_co->ni = t.ni;
}

void	set_co(t_data *data, char *line, int i)
{
	t_obj	*new_co;
	t_mat_t	t;

	read_co(data, &line, i, &t);
	new_co = ft_calloc(1, sizeof(t_obj));
	if (!new_co)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_co->type = CALC_CO;
	new_co->color = t.col;
	mat4_initial(&t.trans);
	mat4_translation(&t.trans, t.center);
	t.rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(t.rot_vec));
	t.final = t.rot;
	t.final = mat4_mult(&t.trans, &t.final);
	ft_objadd_back(&data->objs, new_co);
	set_new_co(t, new_co);
	set_tex(data, &line, i, new_co);
	new_co->next = NULL;
}
