/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_part3_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:31:35 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/12 17:04:51 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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
			while (s[len] && s[len] != '\r'
				&& s[len] != '\n' && !is_space(s[len]))
				len++;
			if (!ft_strncmp(tmp->idx, s, len)
				&& (size_t)len == ft_strlen(tmp->idx))
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
	v->t.rot = mat4_align_vectors((t_vec3){1, 0, 0},
			vec_normalize(v->t.rot_vec));
	v->t.final = mat4_mult(&v->t.rot, &v->t.scale);
	v->t.final = mat4_mult(&v->t.trans, &v->t.final);
	v->str = map_file_fast(v->file, &v->file_size);
	free(v->file);
	if (!v->str)
		clean_exit(data, 1, "Error: Read Fail obj\n", 0);
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
	if (**line && !is_space(**line)
		&& **line != '\n' && **line != '\0' && ft_isdigit(**line))
		v->t.opacity = rt_atod(line);
	skip_spaces(line);
	if (**line && !is_space(**line)
		&& **line != '\n' && **line != '\0' && ft_isdigit(**line))
		v->t.ni = rt_atod(line);
	skip_spaces(line);
	set_vars_obj2(data, line, v);
	set_vars_obj3(data, v);
	data->mtl_info = NULL;
}
