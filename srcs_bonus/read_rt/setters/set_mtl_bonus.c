/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_mtl_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 16:34:13 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/10 15:13:38 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	error_map_mtl(t_vars_obj v_obj, t_data *data, int i)
{
	if (i == 0)
		printf("\rParsing OBJ: [100%%] - OK.\n");
	munmap(v_obj.str, v_obj.file_size);
	free(v_obj.v);
	free(v_obj.vn);
	free(v_obj.vt);
	if (i == 1)
		clean_exit(data, 1, "Error: Read Fail mtl or realloc fail\n", 0);
}

void	map_mtl(char *filename, t_data *data, t_vars_obj *v, t_vars_obj v_obj)
{
	int		i;
	char	*file;

	i = 0;
	while (filename[i])
	{
		if (filename[i] == '\n' || filename[i] == '\r')
			break ;
		i++;
	}
	file = calloc(sizeof(char), (i + 1));
	file = strncpy(file, filename, i);
	v->str = map_file_fast(file, &v->len);
	free(file);
	if (!v->str)
		error_map_mtl(v_obj, data, 1);
	v->end_ptr = v->str + v->len;
	v->step = v->len / 60;
	if (v->step == 0)
		v->step = 1;
	v->next = v->step;
}

void	skip_mtl_option(char **ptr)
{
	if (!ft_strncmp(*ptr, "-", 1))
	{
		while (**ptr)
		{
			if (**ptr == '/')
				break ;
			*ptr += 1;
		}
	}
}

void	mat_ni_ro(int i, t_mtl_info **mtl_node, char **ptr)
{
	if (!ft_strncmp("Ni ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->ni = parse_double(ptr, 0);
		if ((*mtl_node)->ni > 10.0)
			(*mtl_node)->ni = 10.0;
		if ((*mtl_node)->ni < 0.001)
			(*mtl_node)->ni = 0.001;
	}
	else if (!ft_strncmp("Ro ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->rought = parse_double(ptr, 0);
		if ((*mtl_node)->rought > 1.0)
			(*mtl_node)->rought = 1.0;
		if ((*mtl_node)->rought < 0.0)
			(*mtl_node)->rought = 0.0;
	}
}

void	mat_rgb_re(int i, t_mtl_info **mtl_node, char **ptr)
{
	if (!ft_strncmp("RGB ", *ptr, 4) && i == 1)
	{
		*ptr = *ptr + 4;
		(*mtl_node)->color.r = (uint8_t)parse_double(ptr, 0);
		(*mtl_node)->color.g = (uint8_t)parse_double(ptr, 0);
		(*mtl_node)->color.b = (uint8_t)parse_double(ptr, 0);
		(*mtl_node)->color.a = 255;
		(*mtl_node)->has_col = true;
	}
	else if (!ft_strncmp("Re ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->reflectivity = parse_double(ptr, 0);
		if ((*mtl_node)->reflectivity > 1.0)
			(*mtl_node)->reflectivity = 1.0;
		if ((*mtl_node)->reflectivity < 0.0)
			(*mtl_node)->reflectivity = 0.0;
	}
}

void	mat_tex_bump(int i, t_mtl_info **mtl_node, char **ptr)
{
	char	*bump_start;

	if (!ft_strncmp("map_Kd ", *ptr, 7) && i == 1)
	{
		*ptr += 7;
		if (!ft_strncmp(*ptr, "-s", 2))
			(*mtl_node)->st = ft_atoi(*ptr + 2);
		skip_mtl_option(ptr);
		(*mtl_node)->texc = get_texture_path(ptr);
	}
	else if ((!ft_strncmp("map_bump ", *ptr, 9)
			|| !ft_strncmp("bump ", *ptr, 5)
			|| !ft_strncmp("map_Bump ", *ptr, 9)) && i == 1)
	{
		bump_start = *ptr;
		if (!ft_strncmp(bump_start, "map_bump ", 9)
			|| !ft_strncmp("map_Bump ", *ptr, 9))
			*ptr += 9;
		else
			*ptr += 5;
		if (!ft_strncmp(*ptr, "-s", 2))
			(*mtl_node)->sb = ft_atoi(*ptr + 2);
		skip_mtl_option(ptr);
		(*mtl_node)->bumpc = get_texture_path(ptr);
	}
}

void	mat_light(int i, t_mtl_info **mtl_node, char **ptr)
{
	if (!ft_strncmp("Ka ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->ka = parse_vec_fast(ptr, 1);
	}
	else if (!ft_strncmp("Ks ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->ks = parse_vec_fast(ptr, 1);
	}
	else if (!ft_strncmp("Kd ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->kd = parse_vec_fast(ptr, 1);
	}
	else if (!ft_strncmp("Ns ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->ns = parse_double(ptr, 0);
	}
}

void	set_new_mat(int *i,
		t_mtl_info **mtl_info, t_mtl_info **mtl_node, char **ptr)
{
	if (*i == 1)
	{
		(*mtl_node)->next = *mtl_info;
		*mtl_info = *mtl_node;
	}
	*i = 1;
	*mtl_node = malloc(sizeof(t_mtl_info));
	(*mtl_node)->idx = ft_strdup(*ptr + 7);
	(*mtl_node)->tex = NULL;
	(*mtl_node)->ka = (t_vec3){0.2, 0.2, 0.2};
	(*mtl_node)->ks = (t_vec3){1, 1, 1};
	(*mtl_node)->kd = (t_vec3){0.8, 0.8, 0.8};
	(*mtl_node)->ns = 32;
	(*mtl_node)->color = (mlx_color)(uint32_t){0xFFFFFFFF};
	(*mtl_node)->reflectivity = -1.0;
	(*mtl_node)->rought = -1.0;
	(*mtl_node)->opacity = -1.0;
	(*mtl_node)->has_col = false;
	(*mtl_node)->bumpc = NULL;
	(*mtl_node)->texc = NULL;
	(*mtl_node)->bump = NULL;
	(*mtl_node)->ni = 1;
	(*mtl_node)->sb = 1;
	(*mtl_node)->st = 1;
}

void	call_setters_mat(int *i,
		t_mtl_info **mtl_node, char **ptr, t_mtl_info **mtl_info)
{
	if (!ft_strncmp("newmtl ", *ptr, 7))
		set_new_mat(i, mtl_info, mtl_node, ptr);
	mat_light(*i, mtl_node, ptr);
	if (!ft_strncmp("d ", *ptr, 2) && *i == 1)
	{
		*ptr = *ptr + 2;
		(*mtl_node)->opacity = parse_double(ptr, 0);
		if ((*mtl_node)->opacity > 1.0)
			(*mtl_node)->opacity = 1.0;
		if ((*mtl_node)->opacity < 0.0)
			(*mtl_node)->opacity = 0.00001;
	}
	mat_tex_bump(*i, mtl_node, ptr);
	mat_rgb_re(*i, mtl_node, ptr);
	mat_ni_ro(*i, mtl_node, ptr);
}

void	set_mtl_tex_bump(t_vars_obj *v,
		t_mtl_info **mtl_info, t_data *data, t_vars_obj v_obj)
{
	t_mtl_info	*tmp;

	munmap(v->str, v->len);
	printf("\rParsing MTL: [100%%] - OK.\n");
	tmp = *mtl_info;
	data->v_obj = &v_obj;
	while (tmp)
	{
		if (tmp->bumpc)
		{
			printf("%s\n", tmp->bumpc);
			tmp->bump = load_texture(data, tmp->bumpc, NULL, tmp->sb);
			tmp->bumpc = NULL;
		}
		if (tmp->texc)
		{
			printf("%s\n", tmp->texc);
			tmp->tex = load_texture(data, tmp->texc, NULL, tmp->st);
			tmp->texc = NULL;
		}
		tmp = tmp->next;
	}
	data->v_obj = NULL;
}

void	percent_mtl(t_vars_obj *v, char *cursor)
{
	v->pos = cursor - v->str;
	if (v->pos >= v->next)
	{
		printf("\rParsing MTL: [%3lu%%]", (v->pos * 100) / v->len);
		fflush(stdout);
		v->next += v->step;
	}
}

void	read_mtl(char *filename, t_mtl_info **mtl_info,
		t_data *data, t_vars_obj v_obj)
{
	char		*cursor;
	t_vars_obj	v;
	t_mtl_info	*mtl_node;

	map_mtl(filename, data, &v, v_obj);
	cursor = v.str;
	v.i = 0;
	mtl_node = NULL;
	while (cursor < v.end_ptr && *cursor)
	{
		percent_mtl(&v, cursor);
		v.ptr = cursor;
		while (is_space(*v.ptr))
			v.ptr++;
		call_setters_mat(&v.i, &mtl_node, &v.ptr, mtl_info);
		while (cursor < v.end_ptr && *cursor && *cursor != '\n')
			cursor++;
		if (cursor < v.end_ptr && *cursor)
			cursor++;
	}
	if (v.i == 1)
		mtl_node->next = *mtl_info;
	if (v.i == 1)
		*mtl_info = mtl_node;
	set_mtl_tex_bump(&v, mtl_info, data, v_obj);
}
