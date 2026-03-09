/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/09 12:28:18 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

static int	atoi_move(char **s)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	if (**s == '-')
	{
		sign = -1;
		(*s)++;
	}
	while (**s >= '0' && **s <= '9')
	{
		res = res * 10 + (**s - '0');
		(*s)++;
	}
	return (res * sign);
}

void	fill_face_v(char **s, int idx[3][3], int i)
{
	while (**s && !ft_isdigit(**s))
		(*s)++;
	idx[0][i] = atoi_move(s);
	idx[1][i] = 0;
	idx[2][i] = 0;
	if (**s == '/')
	{
		(*s)++;
		if (**s != '/')
			idx[1][i] = atoi_move(s);
		if (**s == '/')
		{
			(*s)++;
			idx[2][i] = atoi_move(s);
		}
	}
	while (**s && !is_space(**s) && **s != '\n')
		(*s)++;
}

static t_triangle	parse_face_fast(char **s, t_vars_obj *v)
{
	t_triangle	tri;
	int			idx[3][3];
	int			i;

	i = -1;
	while (++i < 3)
		fill_face_v(s, idx, i);
	tri.p1 = mat4_mult_vec3(&v->t.final, v->v[idx[0][0] - 1], 1.0);
	tri.p2 = mat4_mult_vec3(&v->t.final, v->v[idx[0][1] - 1], 1.0);
	tri.p3 = mat4_mult_vec3(&v->t.final, v->v[idx[0][2] - 1], 1.0);
	if (idx[1][0] > 0 && v->vt)
	{
		tri.uv1 = (t_vec2){v->vt[idx[1][0] - 1].x, v->vt[idx[1][0] - 1].y};
		tri.uv2 = (t_vec2){v->vt[idx[1][1] - 1].x, v->vt[idx[1][1] - 1].y};
		tri.uv3 = (t_vec2){v->vt[idx[1][2] - 1].x, v->vt[idx[1][2] - 1].y};
	}
	if (idx[2][0] > 0 && v->vn)
		tri.normal = vec_normalize(mat4_mult_vec3(&v->t.final, v->vn[idx[2][0] - 1], 0.0));
	else
		tri.normal = vec_normalize(vec_cross(vec_sub(tri.p2, tri.p1), vec_sub(tri.p3, tri.p1)));
	return (tri);
}

char	*pars_file_n(char **line)
{
	int		len;
	char	*result;

	while (**line && is_space(**line))
		(*line)++;
	len = 0;
	while ((*line)[len] && !is_space((*line)[len]) && (*line)[len] != '\n')
		len++;
	if (len == 0)
		return (NULL);
	result = ft_calloc(len + 1, sizeof(char));
	ft_strlcpy(result, *line, len + 1);
	*line += len;
	return (result);
}

void	set_base_mat(t_mtl_info *mat)
{
	mat->idx = NULL;
	mat->ka = (t_vec3){0.2, 0.2, 0.2};
	mat->ks = (t_vec3){1, 1, 1};
	mat->kd = (t_vec3){0.8, 0.8, 0.8};
	mat->ns = 32;
	mat->ni = -1;
	mat->tex = NULL;
	mat->bump = NULL;
	mat->has_col = false;
	mat->color = (mlx_color)(uint32_t){0xFFFFFFFF};
	mat->reflectivity = -1.0;
	mat->rought = -1.0;
	mat->opacity = -1.0;
}

void	set_new_mtlmat(t_mtl_info *mat, t_mtl_info *tmp)
{
	mat->ka = tmp->ka;
	mat->ns = tmp->ns;
	mat->kd = tmp->kd;
	mat->ks = tmp->ks;
	mat->tex = tmp->tex;
	mat->color = tmp->color;
	mat->reflectivity = tmp->reflectivity;
	mat->rought = tmp->rought;
	mat->has_col = tmp->has_col;
	mat->opacity = tmp->opacity;
	mat->bump = tmp->bump;
}

t_mtl_info	find_mat(t_mtl_info *mtl_info, char *s)
{
	t_mtl_info	mat;
	t_mtl_info	*tmp;
	int			len;

	set_base_mat(&mat);
	tmp = mtl_info;
	len = 0;
	while (tmp)
	{
		if (tmp->idx)
		{
			while (s[len] && s[len] != '\r' && s[len] != '\n' && !is_space(s[len]))
    			len++;
			if (!ft_strncmp(tmp->idx, s, len))
			{
				set_new_mtlmat(&mat, tmp);
				return (mat);
			}
		}
		tmp = tmp->next;
	}
	return (mat);
}

void	set_vars_obj2(t_data *data, char **line, t_vars_obj *v)
{
	v->file = pars_file_n(line);
	v->tex_p = get_texture_path(line);
	v->tex = NULL;
	v->has_tex = false;
	if (v->tex_p)
	{
		v->tex = load_texture(data, v->tex_p, v->file, 1);
		if (v->tex)
			v->has_tex = true;
	}
	mat4_initial(&v->t.trans);
	mat4_translation(&v->t.trans, v->t.center);
	mat4_initial(&v->t.scale);
	mat4_scal(&v->t.scale, (t_vec3){v->t.scaled, v->t.scaled, v->t.scaled});
	v->t.rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(v->t.rot_vec));
	v->t.final = mat4_mult(&v->t.rot, &v->t.scale);
	v->t.final = mat4_mult(&v->t.trans, &v->t.final);
	v->str = map_file_fast(v->file, &v->file_size);
	free(v->file);
	if (!v->str)
	{
		free(v->file);
		clean_exit(data, 1, "Error: Read Fail obj\n", 0);
	}
}

void	set_vars_obj3(t_data *data, t_vars_obj *v)
{
	v->c_v = BUFFER_SIZE;
	v->c_vn = BUFFER_SIZE;
	v->c_vt = BUFFER_SIZE;
	v->v = malloc(sizeof(t_vec3) * v->c_v);
	v->vn = malloc(sizeof(t_vec3) * v->c_vn);
	v->vt = malloc(sizeof(t_vec3) * v->c_vt);
	if (!v->v || !v->vn || !v->vt)
	{
		free(v->v);
		free(v->vn);
		free(v->vt);
		clean_exit(data, 1, "Malloc fail\n", 0);
	}
	v->idx[0] = 0;
	v->idx[1] = 0;
	v->idx[2] = 0;
	v->len = v->file_size;
	v->step = v->len / 60;
	if (v->step == 0)
		v->step = 1;
	v->next = v->step;
}

void	set_vars_obj(t_data *data, int i, char **line, t_vars_obj *v)
{
	*line += 1;
	v->file_size = 0;
	check_missing_info(data, *line, i);
	v->t.center = parse_vec3(line, data, i);
	v->t.rot_vec = parse_vec3(line, data, i);
	v->t.col = parse_color(line, data, i);
	v->t.scaled = rt_atod(line);
	if (v->t.scaled <= 0)
		v->t.scaled = 1.0;
	v->t.reflectivity = rt_atod(line);
	v->t.rought = parse_roughness(line);
	skip_spaces(line);
	v->t.opacity = 1.0;
	v->t.ni = 1.0;
	if (**line && !is_space(**line) && **line != '\n' && **line != '\0' && ft_isdigit(**line))
		v->t.opacity = rt_atod(line);
	skip_spaces(line);
	if (**line && !is_space(**line) && **line != '\n' && **line != '\0' && ft_isdigit(**line))
		v->t.ni = rt_atod(line);
	skip_spaces(line);
	set_vars_obj2(data, line, v);
	set_vars_obj3(data, v);
	data->mtl_info = NULL;
}

void	set_obj_type2(char **c, t_vars_obj *v, t_data *data)
{
	if (*(*c + 1) == 't')
	{
		*c += 3;
		if (v->idx[2] >= v->c_vt)
		{
			v->c_vt *= 2;
			v->vt = realloc(v->vt, sizeof(t_vec3) * v->c_vt);
			if (!v->vt)
				error_map_mtl(*v, data, 1);
		}
		v->vt[v->idx[2]++] = parse_vec_fast(c, 2);
	}
}

void	set_obj_type(char **c, t_vars_obj *v, t_data *data)
{
	if (*(*c + 1) == ' ')
	{
		*c += 2;
		if (v->idx[0] >= v->c_v)
		{
			v->c_v *= 2;
			v->v = realloc(v->v, sizeof(t_vec3) * v->c_v);
			if (!v->vt)
				error_map_mtl(*v, data, 1);
		}
		v->v[v->idx[0]++] = parse_vec_fast(c, 1);
	}
	else if (*(*c + 1) == 'n')
	{
		*c += 3;
		if (v->idx[1] >= v->c_vn)
		{
			v->c_vn *= 2;
			v->vn = realloc(v->vn, sizeof(t_vec3) * v->c_vn);
			if (!v->vt)
				error_map_mtl(*v, data, 1);
		}
		v->vn[v->idx[1]++] = parse_vec_fast(c, 1);
	}
	set_obj_type2(c, v, data);
}

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

void	face_obj(t_vars_obj *v, char  **c, t_data *data, t_mtl_info mat)
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
