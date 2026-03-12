/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:34:44 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_obj_tr2(t_vars_obj *v, t_mtl_info mat, t_data *data)
{
	if (mat.ni > 0.0)
		v->new->ni = mat.ni;
	if (mat.opacity >= 0.0)
		v->new->opacity = mat.opacity;
	if (mat.has_col)
		v->new->color = mat.color;
	if (mat.tex)
	{
		v->new->tex = mat.tex;
		v->new->has_texture = true;
	}
	if (mat.bump)
	{
		v->new->bump = mat.bump;
		v->new->has_bump = true;
	}
	else
		v->new->has_bump = false;
	data->objs = v->new;
}

void	set_obj_tr(t_vars_obj *v, char **c, t_mtl_info mat, t_data *data)
{
	v->new->tri = parse_face_fast(c, v);
	v->new->type = CALC_TR;
	v->new->color = v->t.col;
	v->new->has_texture = v->has_tex;
	if (v->has_tex)
		v->new->tex = v->tex;
	v->new->reflectivity = v->t.reflectivity;
	v->new->rought = v->t.rought;
	v->new->next = data->objs;
	v->new->opacity = v->t.opacity;
	v->new->ni = v->t.ni;
	v->new->ks = mat.ks;
	v->new->kd = mat.kd;
	v->new->ka = mat.ka;
	v->new->ns = mat.ns;
	if (mat.reflectivity >= 0.0)
		v->new->reflectivity = mat.reflectivity;
	if (mat.rought >= 0.0)
		v->new->rought = mat.rought;
	set_obj_tr2(v, mat, data);
}

void	percent_obj(t_vars_obj *v, char *c)
{
	v->pos = c - v->str;
	if (v->pos >= v->next)
	{
		printf("\rParsing OBJ: [%3lu%%]", (v->pos * 100) / v->len);
		fflush(stdout);
		v->next += v->step;
	}
}

void	face_obj(t_vars_obj *v, char **c, t_data *data, t_mtl_info mat)
{
	*c += 2;
	v->new = ft_calloc(1, sizeof(t_obj));
	if (v->new)
		set_obj_tr(v, c, mat, data);
}

void	set_o(t_data *data, char *line, int i)
{
	t_vars_obj	v;
	t_mtl_info	mat;
	char		*c;

	set_vars_obj(data, i, &line, &v);
	v.end_ptr = v.str + v.file_size;
	c = v.str;
	set_base_mat(&mat);
	while (c < v.end_ptr && *c)
	{
		percent_obj(&v, c);
		if (!ft_strncmp(c, "mtllib ", 7) && !data->mtl_info)
			read_mtl(c + 7, &data->mtl_info, data, v);
		if (!ft_strncmp(c, "usemtl ", 7) && data->mtl_info)
			mat = find_mat(data->mtl_info, c + 7);
		if (*c == 'v')
			set_obj_type(&c, &v, data);
		else if (*c == 'f' && is_space(*(c + 1)))
			face_obj(&v, &c, data, mat);
		while (c < v.end_ptr && *c && *c != '\n')
			c++;
		if (c < v.end_ptr && *c)
			c++;
	}
	error_map_mtl(v, data, 0);
}
