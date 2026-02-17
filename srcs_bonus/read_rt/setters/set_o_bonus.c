/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/17 18:43:03 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>
#include <sys/stat.h>
#include <sys/mman.h>

static char *map_file_fast(char *filename, size_t *size)
{
	int         fd;
	struct stat st;
	char        *ptr;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	if (fstat(fd, &st) < 0)
	{
		close(fd);
		return (NULL);
	}
	*size = st.st_size;
	ptr = mmap(NULL, *size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED)
		return (NULL);
	return (ptr);
}

static double parse_double_fast(char **s)
{
	double		res;
	double		sign;
	long long	fraction;
	long long	divisor;
	bool		has_fraction;

	res = 0;
	sign = 1;
	fraction = 0;
	divisor = 1;
	has_fraction = false;
	skip_spaces(s);
	if (**s == '-')
	{
		sign = -1.0;
		(*s)++;
	}
	while (**s >= '0' && **s <= '9')
	{
		res = res * 10.0 + (**s - '0');
		(*s)++;
	}
	if (**s == '.')
	{
		(*s)++;
		while (**s >= '0' && **s <= '9')
		{
			fraction = fraction * 10 + (**s - '0');
			divisor *= 10;
			(*s)++;
			has_fraction = true;
		}
	}
	if (has_fraction)
		res += (double)fraction / (double)divisor;
	return (res * sign);
}

static t_vec3	parse_vec_fast(char **s, int type)
{
	t_vec3	p;

	p.x = parse_double_fast(s);
	p.y = parse_double_fast(s);
	if (type == 1)
		p.z = parse_double_fast(s);
	else
		p.z = 0;
	return (p);
}

static int	fast_atoi_move(char **s)
{
	int	res;

	res = ft_atoi(*s);
	while (**s && ft_isdigit(**s))
		(*s)++;
	return (res);
}

static t_triangle	parse_face_fast(char **s, t_vars_obj *v)
{
	t_triangle	tri;
	int			idx[3][3];
	int			i;

	i = -1;
	while (++i < 3)
	{
		while (**s && !ft_isdigit(**s))
			(*s)++;
		idx[0][i] = fast_atoi_move(s);
		idx[1][i] = 0;
		idx[2][i] = 0;
		if (**s == '/')
		{
			(*s)++;
			if (**s != '/')
				idx[1][i] = fast_atoi_move(s);
			if (**s == '/')
			{
				(*s)++;
				idx[2][i] = fast_atoi_move(s);
			}
		}
		while (**s && !is_space(**s) && **s != '\n')
			(*s)++;
	}
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

static char	*pars_file_n(char **line)
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

void	set_o(t_data *data, char *line, int i)
{
	t_vars_obj	v;
	size_t		file_size = 0;
	char		*end_ptr;
	char		*c;
	int			c_v;
	int			c_vn;
	int			c_vt;

	c_v = BUFFER_SIZE;
	c_vn = BUFFER_SIZE;
	c_vt = BUFFER_SIZE;
	line++;
	check_missing_info(data, line, i);
	v.t.center = parse_vec3(&line, data, i);
	v.t.rot_vec = parse_vec3(&line, data, i);
	v.t.col = parse_color(&line, data, i);
	v.t.scaled = rt_atod(&line);
	if (v.t.scaled <= 0)
		v.t.scaled = 1.0;
	v.t.reflectivity = rt_atod(&line);
	v.t.rought = parse_roughness(&line);
	skip_spaces(&line);
	v.file = pars_file_n(&line);
	v.tex_p = get_texture_path(&line);
	v.tex = NULL;
	v.has_tex = false;
	if (v.tex_p)
	{
		v.tex = load_texture(data, v.tex_p, v.file);
		if (v.tex)
			v.has_tex = true;
	}
	mat4_initial(&v.t.trans);
	mat4_translation(&v.t.trans, v.t.center);
	mat4_initial(&v.t.scale);
	mat4_scal(&v.t.scale, (t_vec3){v.t.scaled, v.t.scaled, v.t.scaled});
	v.t.rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(v.t.rot_vec));
	v.t.final = mat4_mult(&v.t.rot, &v.t.scale);
	v.t.final = mat4_mult(&v.t.trans, &v.t.final);
	v.str = map_file_fast(v.file, &file_size);
	if (!v.str)
	{
		free(v.file);
		clean_exit(data, 1, "Error: Read Fail\n", 0);
	}
	end_ptr = v.str + file_size;
	free(v.file);
	v.v = malloc(sizeof(t_vec3) * c_v);
	v.vn = malloc(sizeof(t_vec3) * c_vn);
	v.vt = malloc(sizeof(t_vec3) * c_vt);
	if (!v.v || !v.vn || !v.vt)
		clean_exit(data, 1, "Malloc fail\n", 0);
	v.idx[0] = 0;
	v.idx[1] = 0;
	v.idx[2] = 0;
	v.len = file_size;
	v.step = v.len / 60;
	if (v.step == 0)
		v.step = 1;
	v.next = v.step;
	c = v.str;
	while (c < end_ptr && *c)
	{
		v.pos = c - v.str;
		if (v.pos >= v.next)
		{
			printf("\rParsing OBJ: [%3lu%%]", (v.pos * 100) / v.len);
			fflush(stdout);
			v.next += v.step;
		}
		if (*c == 'v')
		{
			if (*(c + 1) == ' ')
			{
				c += 2;
				if (v.idx[0] >= c_v)
				{
					c_v *= 2;
					v.v = realloc(v.v, sizeof(t_vec3) * c_v);
				}
				v.v[v.idx[0]++] = parse_vec_fast(&c, 1);
			}
			else if (*(c + 1) == 'n')
			{
				c += 3;
				if (v.idx[1] >= c_vn)
				{
					c_vn *= 2;
					v.vn = realloc(v.vn, sizeof(t_vec3) * c_vn);
				}
				v.vn[v.idx[1]++] = parse_vec_fast(&c, 1);
			}
			else if (*(c + 1) == 't')
			{
				c += 3;
				if (v.idx[2] >= c_vt)
				{
					c_vt *= 2;
					v.vt = realloc(v.vt, sizeof(t_vec3) * c_vt);
				}
				v.vt[v.idx[2]++] = parse_vec_fast(&c, 2);
			}
		}
		else if (*c == 'f' && is_space(*(c + 1)))
		{
			c += 2;
			v.new = ft_calloc(1, sizeof(t_obj));
			if (v.new)
			{
				v.new->tri = parse_face_fast(&c, &v);
				v.new->type = CALC_TR;
				v.new->color = v.t.col;
				v.new->has_texture = v.has_tex;
				if (v.has_tex)
					v.new->tex = v.tex;
				v.new->reflectivity = v.t.reflectivity;
				v.new->rought = v.t.rought;
				ft_objadd_back(&data->objs, v.new);
			}
		}
		while (c < end_ptr && *c && *c != '\n')
			c++;
		if (c < end_ptr && *c)
			c++;
	}
	printf("\rParsing OBJ: [100%%] - OK.\n");
	munmap(v.str, file_size);
	free(v.v);
	free(v.vn);
	free(v.vt);
}
