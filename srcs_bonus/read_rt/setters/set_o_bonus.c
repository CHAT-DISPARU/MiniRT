/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/03 16:28:32 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

char	*pars_file_n(char *line)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_calloc(ft_strlen(line), 1);
	if (!result)
		return (NULL);
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '/' || line[i] == '.'))
	{
		result[i] = line[i];
		i++;
	}
	return (result);
}

void	face_node(char *line, int fd, t_data *data, t_face_c **face_c)
{
	t_face_c	*new;

	new = malloc(sizeof(t_face_c));
	if (!new)
	{
		free(line);
		get_next_line(fd, 1);
		close (fd);
		clean_exit(data, 1, "Error: The file is a directory\n", 0);
	}
	line++;
	skip_spaces(&line);
	new->point.x = rt_atod(&line);
	skip_spaces(&line);
	new->point.y = rt_atod(&line);
	skip_spaces(&line);
	new->point.z = rt_atod(&line);
	new->next = NULL;
	ft_faceadd_back(face_c, new);
}

void	fill_face(int fd, t_face_c **face_c, t_data *data)
{
	char		*line;

	line = get_next_line(fd, 0);
	while (line)
	{
		if (line[0] == 'v')
		{
			while (line && line[0] == 'v')
			{
				face_node(line, fd, data, face_c);
				free(line);
				line = get_next_line(fd, 0);
			}
			free(line);
			return ;
		}
		free(line);
		line = get_next_line(fd, 0);
	}
}

t_vec3	ft_get_vec_s(int i, t_face_c *face_c)
{
	int j;
	t_face_c	*current;

	current = face_c;
	if (i <= 0)
		return ((t_vec3){0, 0, 0});
	j = 1;
	while (j < i)
	{
		if (current == NULL)
			return ((t_vec3){0, 0, 0});
		current = current->next;
		j++;
	}
	return (current->point);
}

t_triangle	find_tri(char *line, t_face_c *face_c, t_mat_t t)
{
	int	i;
	t_triangle	tri;

	i = 0;
	tri.p1 = (t_vec3){0, 0, 0};
	tri.p2 = (t_vec3){0, 0, 0};
	tri.p3 = (t_vec3){0, 0, 0};
	while (*line && *line != ' ')
		line++;
	tri.p1 = mat4_mult_vec3(&t.final, ft_get_vec_s(atoi(line), face_c), 1);
	skip_spaces(&line);
	while (*line && *line != ' ')
		line++;
	tri.p2 = mat4_mult_vec3(&t.final, ft_get_vec_s(atoi(line), face_c), 1);
	skip_spaces(&line);
	while (*line && *line != ' ')
		line++;
	tri.p3 = mat4_mult_vec3(&t.final, ft_get_vec_s(atoi(line), face_c), 1);
	return (tri);
}

void	new_tr_obj(t_file_info f, t_data *data, t_face_c *face_c, t_mat_t t)
{
	t_obj		*new_tr;

	new_tr = malloc(sizeof(t_obj));
	if (!new_tr)
	{
		free(f.line_o);
		get_next_line(f.fd, 1);
		close(f.fd);
		clean_exit(data, 1, "malloc fail\n", 0);
	}
	new_tr->tri = find_tri(f.line_o++, face_c, t);
	new_tr->type = CALC_TR;
	new_tr->color = t.col;
	new_tr->next = NULL;
	ft_objadd_back(&data->objs, new_tr);
}

void	set_o(t_data *data, char *line, int i)
{
	t_mat_t		t;
	t_face_c	*face_c;
	char		*filename;
	char		buffer[1];
	t_file_info	f;

	line++;
	face_c = NULL;
	check_missing_info(data, line, i);
	t.center = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	t.rot_vec = parse_vec3(&line, data, i);
	check_norm_vec(data, t.rot_vec, i);
	check_missing_info(data, line, i);
	t.col = parse_color(&line, data, i);
	check_missing_info(data, line, i);
	skip_spaces(&line);
	filename = pars_file_n(line);
	if (!filename)
		clean_exit(data, 1, "malloc fail or file not found \n", 0);
	f.fd = open (filename, O_RDONLY);
	line += ft_strlen(filename);
	free(filename);
	if (f.fd < 0)
		clean_exit(data, 1, "malloc fail or file not found\n", 0);
	check_extra_info(data, line, i);
	if (read(data->scene_fd, buffer, 0) == -1)
	{
		close(f.fd);
		clean_exit(data, 1, "Error: The file is a directory\n", 0);
	}
	fill_face(f.fd, &face_c, data);
	mat4_initial(&t.trans);
	mat4_translation(&t.trans, t.center);
	t.rot = mat4_align_vectors((t_vec3){0, 1, 0}, vec_normalize(t.rot_vec));
	t.final = mat4_mult(&t.trans, &t.rot);
	f.line_o = get_next_line(f.fd, 0);
	while (f.line_o)
	{                                                                                                                                                                                                                                                                                                                  
		if (f.line_o[0] == 'f' && is_space(f.line_o[1]))
			new_tr_obj(f, data, face_c, t);
		free(f.line_o);
		f.line_o = get_next_line(f.fd, 0);
	}
	close(f.fd);
	t_face_c	*tmp;
	while (face_c)
	{
		tmp = face_c->next;
		free(face_c);
		face_c = tmp;
	}
}
