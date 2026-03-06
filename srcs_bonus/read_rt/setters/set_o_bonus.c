/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 12:00:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/06 11:28:53 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>

char	*ft_strncat(char *dest, char *src, unsigned int nb)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i ++;
	while (j < nb && src[j] != '\0')
	{
		dest[i + j] = src[j];
		j ++;
	}
	dest[i + j] = '\0';
	return (dest);
}

static char	*map_file_fast(char *filename, size_t *size)
{
	int			fd;
	struct stat	st;
	char		*ptr;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	if (fstat(fd, &st) < 0)
	{
		close(fd);
		return (NULL);
	}
	*size = st.st_size;
	ptr = mmap(NULL, *size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED)
		return (NULL);
	return (ptr);
}

static double	parse_double_fast(char **s)
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

static inline int	atoi_move(char **s)
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

void	read_mtl(char *filename, t_mtl_info **mtl_info, t_data *data)
{
	char	*file;
	int		i;
	char		*cursor;
	t_vars_obj	v;
	char		*ptr;
	t_mtl_info	*mtl_node;
	t_vec3		vec;
	char		*end_ptr;

	i = 0;
	while (filename[i])
	{
		if (filename[i] == '\n' || filename[i] == '\r')
			break ;
		i++;
	}
	file = calloc(sizeof(char), (i + 1));
	file = strncpy(file, filename, i);
	v.str = map_file_fast(file, &v.len);
	if (!v.str)
	{
		free(file);
		clean_exit(data, 1, "Error: Read Fail mtl\n", 0);
	}
	free(file);
	end_ptr = v.str + v.len;
	i = 0;
	v.step = v.len / 60;
	if (v.step == 0)
		v.step = 1;
	v.next = v.step;
	cursor = v.str;
	while (cursor < end_ptr && *cursor)
	{
		v.pos = cursor - v.str;
		if (v.pos >= v.next)
		{
			printf("\rParsing MTL: [%3lu%%]", (v.pos * 100) / v.len);
			fflush(stdout);
			v.next += v.step;
		}
		ptr = cursor;
		while (is_space(*ptr))
			ptr++;
		if (!ft_strncmp("newmtl ", ptr, 7))
		{
			if (i == 1)
			{
				mtl_node->next = *mtl_info;
				*mtl_info = mtl_node;
			}
			i = 1;
			mtl_node = NULL;
			mtl_node = malloc(sizeof(t_mtl_info));
			mtl_node->idx = ft_strdup(ptr + 7);
			mtl_node->tex = NULL;
			mtl_node->ka = (t_vec3){0.2, 0.2, 0.2};
			mtl_node->ks = (t_vec3){1, 1, 1};
			mtl_node->kd = (t_vec3){0.8, 0.8, 0.8};
			mtl_node->ns = 32;
			mtl_node->color = (mlx_color)(uint32_t){0xFFFFFFFF};
			mtl_node->reflectivity = -1.0;
			mtl_node->rought = -1.0;
			mtl_node->opacity = -1.0;
			mtl_node->has_col = false;
			mtl_node->bumpc = NULL;
			mtl_node->texc = NULL;
			mtl_node->bump = NULL;
			mtl_node->ni = 1;
		}
		else if (!ft_strncmp("Ka ", ptr, 3) && i == 1)
		{
			ptr = ptr + 3;
			mtl_node->ka = parse_vec_fast(&ptr, 1);
		}
		else if (!ft_strncmp("Ks ", ptr, 3) && i == 1)
		{
			ptr = ptr + 3;
			mtl_node->ks = parse_vec_fast(&ptr, 1);
		}
		else if (!ft_strncmp("Kd ", ptr, 3) && i == 1)
		{
			ptr = ptr + 3;
			mtl_node->kd = parse_vec_fast(&ptr, 1);
		}
		else if (!ft_strncmp("Ns ", ptr, 3) && i == 1)
		{
			ptr = ptr + 3;
			vec.x = parse_double_fast(&ptr);
			mtl_node->ns = vec.x;
		}
		else if (!ft_strncmp("d ", ptr, 2) && i == 1)
		{
			ptr = ptr + 2;
			mtl_node->opacity = parse_double_fast(&ptr);
			if (mtl_node->opacity > 1.0)
				mtl_node->opacity = 1.0;
			if (mtl_node->opacity < 0.0)
				mtl_node->opacity = 0.0;
		}
		else if (!ft_strncmp("map_Kd ", ptr, 7) && i == 1)
		{
			ptr += 7;
			int s = 1;
			if (!ft_strncmp(ptr, "-s", 2))
				s = ft_atoi(ptr + 2);
			if (!ft_strncmp(ptr, "-", 1))
			{
				while (*ptr)
				{
					if (*ptr == '/')
						break ;
					ptr++;
				}
			}
			mtl_node->texc = get_texture_path(&ptr);
		}
		else if ((!ft_strncmp("map_bump ", ptr, 9) || !ft_strncmp("bump ", ptr, 5) || !ft_strncmp("map_Bump ", ptr, 9)) && i == 1)
		{
			char *bump_start = ptr;
			if (!ft_strncmp(bump_start, "map_bump ", 9) || !ft_strncmp("map_Bump ", ptr, 9))
				ptr += 9;
			else
				ptr += 5;
			if (!ft_strncmp(ptr, "-", 1))
			{
				while (*ptr)
				{
					if (*ptr == '/')
						break ;
					ptr++;
				}
			}
			mtl_node->bumpc = get_texture_path(&ptr);
		}
		else if (!ft_strncmp("RGB ", ptr, 4) && i == 1)
		{
			ptr = ptr + 4;
			mtl_node->color.r = (uint8_t)parse_double_fast(&ptr);
			mtl_node->color.g = (uint8_t)parse_double_fast(&ptr);
			mtl_node->color.b = (uint8_t)parse_double_fast(&ptr);
			mtl_node->color.a = 255;
			mtl_node->has_col = true;
		}
		else if (!ft_strncmp("Re ", ptr, 3) && i == 1)
		{
			ptr = ptr + 3;
			mtl_node->reflectivity = parse_double_fast(&ptr);
			if (mtl_node->reflectivity > 1.0)
				mtl_node->reflectivity = 1.0;
			if (mtl_node->reflectivity < 0.0)
				mtl_node->reflectivity = 0.0;
		}
		else if (!ft_strncmp("Ni ", ptr, 3) && i == 1)
		{
			ptr = ptr + 3;
			mtl_node->ni = parse_double_fast(&ptr);
			if (mtl_node->ni > 10.0)
				mtl_node->ni = 10.0;
			if (mtl_node->ni < 0.001)
				mtl_node->ni = 0.001;
		}
		else if (!ft_strncmp("Ro ", ptr, 3) && i == 1)
		{
			ptr = ptr + 3;
			mtl_node->rought = parse_double_fast(&ptr);
			if (mtl_node->rought > 1.0)
				mtl_node->rought = 1.0;
			if (mtl_node->rought < 0.0)
				mtl_node->rought = 0.0;
		}
		while (cursor < end_ptr && *cursor && *cursor != '\n')
			cursor++;
		if (cursor < end_ptr && *cursor)
			cursor++;
	}
	if (i == 1)
	{
		mtl_node->next = *mtl_info;
		*mtl_info = mtl_node;
	}
	munmap(v.str, v.len);
	t_mtl_info	*tmp;
	printf("\rParsing MTL: [100%%] - OK.\n");
	tmp = *mtl_info;
	while (tmp)
	{
		if (tmp->bumpc)
		{
			printf("%s\n", tmp->bumpc);
			tmp->bump = load_texture(data, tmp->bumpc, NULL);
			tmp->bumpc = NULL;
		}
		if (tmp->texc)
		{
			printf("%s\n", tmp->texc);
			tmp->tex = load_texture(data, tmp->texc, NULL);
			tmp->texc = NULL;
		}
		tmp = tmp->next;
	}
}

t_mtl_info	find_mat(t_mtl_info *mtl_info, char *s)
{
	t_mtl_info	mat;
	t_mtl_info	*tmp;
	int			len;

	mat.ka = (t_vec3){0.2, 0.2, 0.2};
	mat.ks = (t_vec3){1, 1, 1};
	mat.kd = (t_vec3){0.8, 0.8, 0.8};
	mat.ns = 32;
	mat.ni = 1;
	mat.tex = NULL;
	mat.bump = NULL;
	mat.has_col = false;
	mat.color = (mlx_color)(uint32_t){0xFFFFFFFF};
	mat.reflectivity = -1.0;
	mat.rought = -1.0;
	mat.opacity = -1.0;
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
				mat.ka = tmp->ka;
				mat.ns = tmp->ns;
				mat.kd = tmp->kd;
				mat.ks = tmp->ks;
				mat.tex = tmp->tex;
				mat.color = tmp->color;
				mat.reflectivity = tmp->reflectivity;
				mat.rought = tmp->rought;
				mat.has_col = tmp->has_col;
				mat.opacity = tmp->opacity;
				mat.bump = tmp->bump;
				return (mat);
			}
		}
		tmp = tmp->next;
	}
	return (mat);
}

void	set_o(t_data *data, char *line, int i)
{
	t_vars_obj	v;
	t_mtl_info	mat;
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
	double	opacity = 1.0;
	double	ni = 1.0;
	if (*line && !is_space(*line) && *line != '\n' && *line != '\0' && ft_isdigit(*line))
		opacity = rt_atod(&line);
	skip_spaces(&line);
	if (*line && !is_space(*line) && *line != '\n' && *line != '\0' && ft_isdigit(*line))
		ni = rt_atod(&line);
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
		clean_exit(data, 1, "Error: Read Fail obj\n", 0);
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
	data->mtl_info = NULL;
	mat.idx = NULL;
	mat.ka = (t_vec3){0.2, 0.2, 0.2};
	mat.ks = (t_vec3){1, 1, 1};
	mat.kd = (t_vec3){0.8, 0.8, 0.8};
	mat.ns = 32;
	mat.tex = NULL;
	mat.ni = ni;
	mat.bump = NULL;
	mat.has_col = false;
	mat.color = (mlx_color)(uint32_t){0xFFFFFFFF};
	mat.reflectivity = -1.0;
	mat.rought = -1.0;
	mat.opacity = opacity;
	while (c < end_ptr && *c)
	{
		v.pos = c - v.str;
		if (v.pos >= v.next)
		{
			printf("\rParsing OBJ: [%3lu%%]", (v.pos * 100) / v.len);
			fflush(stdout);
			v.next += v.step;
		}
		if (!ft_strncmp(c, "mtllib ", 7) && !data->mtl_info)
			read_mtl(c + 7, &data->mtl_info, data);
		if (!ft_strncmp(c, "usemtl ", 7) && data->mtl_info)
			mat = find_mat(data->mtl_info, c + 7);
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
				v.new->next = data->objs;
				v.new->opacity = 1.0;
				v.new->ks = mat.ks;
				v.new->kd = mat.kd;
				v.new->ka = mat.ka;
				v.new->ns = mat.ns;
				if (mat.reflectivity >= 0.0)
					v.new->reflectivity = mat.reflectivity;
				if (mat.rought >= 0.0)
					v.new->rought = mat.rought;
				if (mat.opacity >= 0.0)
            		v.new->opacity = mat.opacity;
				if (mat.has_col)
					v.new->color = mat.color;
				if (mat.tex)
				{
					v.new->tex = mat.tex;
					v.new->has_texture = true;
				}
				if (mat.bump)
				{
					v.new->bump = mat.bump;
					v.new->has_bump = true;
				}
				else
					v.new->has_bump = false;
				data->objs = v.new;
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
