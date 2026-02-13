/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/13 16:17:11 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

char	*pars_file_n(char **line)
{
    int		i;
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
    if (!result)
        return (NULL);
    i = 0;
    while (i < len)
    {
        result[i] = **line;
        (*line)++;
        i++;
    }
    return (result);
}

void	face_node(char *line, t_face_c **list)
{
	t_face_c	*new;

	new = malloc(sizeof(t_face_c));
	if (!new)
		return ;
	skip_spaces(&line);
	new->point.x = rt_atod(&line);
	skip_spaces(&line);
	new->point.y = rt_atod(&line);
	skip_spaces(&line);
	new->point.z = rt_atod(&line);
	new->next = NULL;
	ft_faceadd_back(list, new);
}

void	fill_obj_data(int fd, t_face_c **v_l, t_face_c **vn_l, t_face_c **vt_l)
{
	char	*line;

	line = get_next_line(fd, 0);
	while (line)
	{
		if (line[0] == 'v' && line[1] == ' ')
			face_node(line + 1, v_l);
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
			face_node(line + 2, vn_l);
		else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
			face_node(line + 2, vt_l);
		free(line);
		line = get_next_line(fd, 0);
	}
}

t_vec3	get_vec_from_list(int i, t_face_c *list)
{
	int	j;

	if (i <= 0 || !list)
		return ((t_vec3){0, 0, 0});
	j = 1;
	while (j < i && list->next)
	{
		list = list->next;
		j++;
	}
	return (list->point);
}

t_triangle	parse_face_line(char *line, t_face_c *v_l, t_face_c *vn_l, t_face_c *vt_l,t_mat_t t)
{
	t_triangle	tri;
	int			v_idx[3];
	int			vn_idx[3];
	int			vt_idx[3];
	int			i = 0;

	while (i < 3)
	{
		skip_spaces(&line);
		v_idx[i] = ft_atoi(line);
		vn_idx[i] = 0;
		vt_idx[i] = 0;
		char	*next_space = ft_strchr(line, ' ');
		char	*slash = ft_strchr(line, '/');
		if (slash && (!next_space || slash < next_space))
		{
			if (*(slash + 1) != '/')
				vt_idx[i] = ft_atoi(slash + 1);
			char *second_slash = ft_strchr(slash + 1, '/');
			if (second_slash && (!next_space || second_slash < next_space))
                vn_idx[i] = ft_atoi(second_slash + 1);
		}
		while (*line && *line != ' ' && *line != '\t')
			line++;
		i++;
	}
	tri.p1 = mat4_mult_vec3(&t.final, get_vec_from_list(v_idx[0], v_l), 1.0);
	tri.p2 = mat4_mult_vec3(&t.final, get_vec_from_list(v_idx[1], v_l), 1.0);
	tri.p3 = mat4_mult_vec3(&t.final, get_vec_from_list(v_idx[2], v_l), 1.0);
	if (vt_idx[0] > 0 && vt_l)
    {
        t_vec3 uv1 = get_vec_from_list(vt_idx[0], vt_l);
        t_vec3 uv2 = get_vec_from_list(vt_idx[1], vt_l);
        t_vec3 uv3 = get_vec_from_list(vt_idx[2], vt_l);
        tri.uv1 = (t_vec2){uv1.x, uv1.y};
        tri.uv2 = (t_vec2){uv2.x, uv2.y};
        tri.uv3 = (t_vec2){uv3.x, uv3.y};
    }
	if (vn_idx[0] != 0 && vn_l)
		tri.normal = vec_normalize(mat4_mult_vec3(&t.final, get_vec_from_list(vn_idx[0], vn_l), 0.0));
	else
	{
		t_vec3	e1 = vec_sub(tri.p2, tri.p1);
		t_vec3	e2 = vec_sub(tri.p3, tri.p1);
		tri.normal = vec_normalize(vec_cross(e1, e2));
	}
	return (tri);
}

void	new_tr_obj(char *line, t_data *data, t_face_c *v_l, t_face_c *vn_l, t_face_c *vt_l,t_mat_t t, t_texture *tex, bool has_texture)
{
	t_obj	*new_tr;

	new_tr = ft_calloc(1, sizeof(t_obj));
	if (!new_tr)
		return ;
	new_tr->tri = parse_face_line(line + 1, v_l, vn_l, vt_l, t);
	new_tr->type = CALC_TR;
	new_tr->color = t.col;
	new_tr->has_texture = has_texture;
	if (has_texture)
		new_tr->tex = tex;
	new_tr->reflectivity = t.reflectivity;
	new_tr->rought = t.rought;
	new_tr->next = NULL;
	ft_objadd_back(&data->objs, new_tr);
}

void	free_face_list(t_face_c *list)
{
	t_face_c	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

void	set_o(t_data *data, char *line, int i)
{
	t_mat_t		t;
	t_face_c	*v_list = NULL;
	t_face_c	*vn_list = NULL;
	t_face_c	*vt_list = NULL;
	t_file_info	f;
	t_texture	*tex;
	char		*filename;
	char		*tex_path;
	bool		has_texture;

	line++;
	check_missing_info(data, line, i);
	t.center = parse_vec3(&line, data, i);
	t.rot_vec = parse_vec3(&line, data, i);
	t.col = parse_color(&line, data, i);
	t.scaled = rt_atod(&line);
	t.reflectivity = rt_atod(&line);
	t.rought = parse_roughness(&line);
	if (t.scaled <= 0)
		t.scaled = 1.0;
	skip_spaces(&line);
	filename = pars_file_n(&line);
	tex_path = get_texture_path(&line);
	tex = NULL;
	if (tex_path)
	{
		has_texture = true;
		tex = load_texture(data, tex_path);
	}
	else
		has_texture = false;
	f.fd = open(filename, O_RDONLY);
	if (f.fd < 0)
		clean_exit(data, 1, "Error: File not found\n", 0);
	fill_obj_data(f.fd, &v_list, &vn_list, &vt_list);
	close(f.fd);
	mat4_initial(&t.trans);
	mat4_translation(&t.trans, t.center);
	mat4_initial(&t.scale);
	mat4_scal(&t.scale, (t_vec3){t.scaled, t.scaled, t.scaled});
	t.rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(t.rot_vec));
	t.final = mat4_mult(&t.rot, &t.scale);
	t.final = mat4_mult(&t.trans, &t.final);
	f.fd = open(filename, O_RDONLY);
	f.line_o = get_next_line(f.fd, 0);
	while (f.line_o)
	{
		if (f.line_o[0] == 'f' && (f.line_o[1] == ' ' || f.line_o[1] == '\t'))
			new_tr_obj(f.line_o, data, v_list, vn_list, vt_list, t, tex, has_texture);
		free(f.line_o);
		f.line_o = get_next_line(f.fd, 0);
	}
	close(f.fd);
	free(filename);
	free_face_list(v_list);
	free_face_list(vn_list);
	free_face_list(vt_list);
}
