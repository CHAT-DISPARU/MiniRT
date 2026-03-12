/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_mtl_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:20:52 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:25:43 by gajanvie         ###   ########.fr       */
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
