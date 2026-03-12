/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_mtl_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 16:34:13 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/12 12:07:36 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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

void	read_mtl(char *filename, t_mtl_info **mtl_info,
		t_data *data, t_vars_obj v_obj)
{
	char		*curs;
	t_vars_obj	v;
	t_mtl_info	*mtl_node;

	map_mtl(filename, data, &v, v_obj);
	curs = v.str;
	v.i = 0;
	mtl_node = NULL;
	while (curs < v.end_ptr && *curs)
	{
		percent_mtl(&v, curs);
		v.ptr = curs;
		while (is_space(*v.ptr))
			v.ptr++;
		call_setters_mat(&v.i, &mtl_node, &v.ptr, mtl_info);
		while (curs < v.end_ptr && *curs && *curs != '\n' && *curs != '\r')
			curs++;
		if (curs < v.end_ptr && *curs)
			curs++;
	}
	if (v.i == 1)
		mtl_node->next = *mtl_info;
	if (v.i == 1)
		*mtl_info = mtl_node;
	set_mtl_tex_bump(&v, mtl_info, data, v_obj);
}
