/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_ply_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 11:47:48 by titan             #+#    #+#             */
/*   Updated: 2026/03/02 21:25:14 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>
#include <sys/stat.h>
#include <sys/mman.h>

static int	ply_type_size(t_ply_type type)
{
	if (type == PLY_FLOAT)
		return (4);
	if (type == PLY_DOUBLE)
		return (8);
	if (type == PLY_UCHAR)
		return (1);
	if (type == PLY_INT)
		return (4);
	if (type == PLY_UINT)
		return (4);
	if (type == PLY_SHORT)
		return (2);
	if (type == PLY_USHORT)
		return (2);
	return (0);
}

static t_ply_type	ply_parse_type(char *s)
{
	if (!ft_strncmp(s, "float", 5))
		return (PLY_FLOAT);
	if (!ft_strncmp(s, "double", 6))
		return (PLY_DOUBLE);
	if (!ft_strncmp(s, "uchar", 5))
		return (PLY_UCHAR);
	if (!ft_strncmp(s, "uint8", 5))
		return (PLY_UCHAR);
	if (!ft_strncmp(s, "int", 3))
		return (PLY_INT);
	if (!ft_strncmp(s, "uint", 4))
		return (PLY_UINT);
	if (!ft_strncmp(s, "short", 5))
		return (PLY_SHORT);
	if (!ft_strncmp(s, "ushort", 6))
		return (PLY_USHORT);
	return (PLY_UNKNOWN);
}

static float	ply_read_float(char *ptr, t_ply_type type)
{
	float			f;
	double			d;
	unsigned char	uc;
	int				ii;
	unsigned int	ui;

	if (type == PLY_FLOAT)
	{
		ft_memcpy(&f, ptr, 4);
		return (f);
	}
	if (type == PLY_DOUBLE)
	{
		ft_memcpy(&d, ptr, 8);
		return ((float)d);
	}
	if (type == PLY_UCHAR)
	{
		ft_memcpy(&uc, ptr, 1);
		return ((float)uc);
	}
	if (type == PLY_INT)
	{
		ft_memcpy(&ii, ptr, 4);
		return ((float)ii);
	}
	if (type == PLY_UINT)
	{
		ft_memcpy(&ui, ptr, 4);
		return ((float)ui);
	}
	return (0.0f);
}

static t_ply_header	parse_ply_header(char *mapped, size_t file_size)
{
	t_ply_header	h;
	char			*c;
	char			*end;
	int				in_vertex;
	int				in_face;

	(void)file_size;
	ft_memset(&h, 0, sizeof(t_ply_header));
	h.has_nx = -1;
	h.has_r = -1;
	h.has_s = -1;
	h.is_binary = false;
	in_vertex = 0;
	in_face = 0;
	c = mapped;
	if (ft_strncmp(c, "ply", 3))
		return (h);
	while (*c && *c != '\n')
		c++;
	c++;
	while (*c)
	{
		end = c;
		while (*end && *end != '\n')
			end++;
		if (!ft_strncmp(c, "format binary_little_endian", 27))
			h.is_binary = true;
		else if (!ft_strncmp(c, "element vertex ", 15))
		{
			char *n = c + 15;
			h.vertex_count = 0;
			while (*n >= '0' && *n <= '9')
				h.vertex_count = h.vertex_count * 10 + (*n++ - '0');
			in_vertex = 1;
			in_face = 0;
		}
		else if (!ft_strncmp(c, "element face ", 13))
		{
			char *n = c + 13;
			h.face_count = 0;
			while (*n >= '0' && *n <= '9')
				h.face_count = h.face_count * 10 + (*n++ - '0');
			in_vertex = 0;
			in_face = 1;
		}
		else if (!ft_strncmp(c, "property ", 9) && in_vertex && !in_face)
		{
			char			*p = c + 9;
			t_ply_prop		prop;
			ft_memset(&prop, 0, sizeof(t_ply_prop));
			prop.type = ply_parse_type(p);
			prop.size = ply_type_size(prop.type);
			prop.offset = h.vertex_size;
			while (*p && !is_space(*p))
				p++;
			while (*p && is_space(*p))
				p++;
			int ni = 0;
			while (*p && *p != '\n' && *p != '\r' && ni < 63)
				prop.name[ni++] = *p++;
			prop.name[ni] = '\0';
			if (!ft_strncmp(prop.name, "nx", 2))
				h.has_nx = h.prop_count;
			if (!ft_strncmp(prop.name, "red", 3))
				h.has_r = h.prop_count;
			if (prop.name[0] == 's' && prop.name[1] == '\0')
				h.has_s = h.prop_count;
			h.props[h.prop_count++] = prop;
			h.vertex_size += prop.size;
		}
		else if (!ft_strncmp(c, "end_header", 10))
		{
			h.data_offset = (int)(end + 1 - mapped);
			break ;
		}
		c = end + 1;
	}
	return (h);
}

void	set_ply(t_data *data, char *line, int i)
{
	t_vars_obj		vo;
	int				fd;
	struct stat		st;
	char			*mapped;
	t_ply_header	h;
	t_vec3			*verts;
	t_vec3			*norms;
	mlx_color		*colors;
	t_vec2			*uvs;
	char			*dptr;
	int				j;
	int				k;
	t_obj			*new;

	line++;
	check_missing_info(data, line, i);
	vo.t.center = parse_vec3(&line, data, i);
	vo.t.rot_vec = parse_vec3(&line, data, i);
	vo.t.col = parse_color(&line, data, i);
	vo.t.scaled = rt_atod(&line);
	if (vo.t.scaled <= 0)
		vo.t.scaled = 1.0;
	vo.t.reflectivity = rt_atod(&line);
	vo.t.rought = parse_roughness(&line);
	skip_spaces(&line);
	vo.file = pars_file_n(&line);
	vo.tex_p = get_texture_path(&line);
	vo.tex = NULL;
	vo.has_tex = false;
	if (vo.tex_p)
	{
		vo.tex = load_texture(data, vo.tex_p, vo.file);
		if (vo.tex)
			vo.has_tex = true;
	}
	mat4_initial(&vo.t.trans);
	mat4_translation(&vo.t.trans, vo.t.center);
	mat4_initial(&vo.t.scale);
	mat4_scal(&vo.t.scale, (t_vec3){vo.t.scaled, vo.t.scaled, vo.t.scaled});
	vo.t.rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(vo.t.rot_vec));
	vo.t.final = mat4_mult(&vo.t.rot, &vo.t.scale);
	vo.t.final = mat4_mult(&vo.t.trans, &vo.t.final);
	fd = open(vo.file, O_RDONLY);
	free(vo.file);
	if (fd < 0)
		clean_exit(data, 1, "Error: open ply fail\n", 0);
	if (fstat(fd, &st) < 0)
		clean_exit(data, 1, "Error: fstat ply fail\n", 0);
	mapped = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (mapped == MAP_FAILED)
		clean_exit(data, 1, "Error: mmap ply fail\n", 0);
	h = parse_ply_header(mapped, st.st_size);
	if (!h.is_binary || h.vertex_count == 0 || h.face_count == 0)
		clean_exit(data, 1, "Error: PLY invalid or ASCII\n", 0);
	verts = malloc(sizeof(t_vec3) * h.vertex_count);
	norms = malloc(sizeof(t_vec3) * h.vertex_count);
	colors = malloc(sizeof(mlx_color) * h.vertex_count);
	uvs = malloc(sizeof(t_vec2) * h.vertex_count);
	if (!verts || !norms || !colors || !uvs)
		clean_exit(data, 1, "Malloc ply\n", 0);
	dptr = mapped + h.data_offset;
	j = 0;
	while (j < h.vertex_count)
	{
		float	x = ply_read_float(dptr + h.props[0].offset, h.props[0].type);
		float	y = ply_read_float(dptr + h.props[1].offset, h.props[1].type);
		float	z = ply_read_float(dptr + h.props[2].offset, h.props[2].type);
		verts[j] = mat4_mult_vec3(&vo.t.final, (t_vec3){x, y, z}, 1.0);
		norms[j] = (t_vec3){0, 0, 0};
		if (h.has_nx >= 0)
		{
			float nx = ply_read_float(dptr + h.props[h.has_nx].offset, h.props[h.has_nx].type);
			float ny = ply_read_float(dptr + h.props[h.has_nx + 1].offset, h.props[h.has_nx + 1].type);
			float nz = ply_read_float(dptr + h.props[h.has_nx + 2].offset, h.props[h.has_nx + 2].type);
			norms[j] = vec_normalize(mat4_mult_vec3(&vo.t.final, (t_vec3){nx, ny, nz}, 0.0));
		}
		colors[j] = vo.t.col;
		if (h.has_r >= 0)
		{
			colors[j].r = (uint8_t)ply_read_float(dptr + h.props[h.has_r].offset,
					h.props[h.has_r].type);
			colors[j].g = (uint8_t)ply_read_float(dptr + h.props[h.has_r + 1].offset,
					h.props[h.has_r + 1].type);
			colors[j].b = (uint8_t)ply_read_float(dptr + h.props[h.has_r + 2].offset,
					h.props[h.has_r + 2].type);
			colors[j].a = 255;
		}
		uvs[j] = (t_vec2){0, 0};
		if (h.has_s >= 0)
		{
			uvs[j].x = ply_read_float(dptr + h.props[h.has_s].offset, h.props[h.has_s].type);
			uvs[j].y = ply_read_float(dptr + h.props[h.has_s + 1].offset, h.props[h.has_s + 1].type);
		}
		dptr += h.vertex_size;
		j++;
	}
	printf("Parsing PLY: %d vertices, %d faces\n", h.vertex_count, h.face_count);
	k = 0;
	while (k < h.face_count)
	{
		if (k % (h.face_count / 60 + 1) == 0)
		{
			printf("\rParsing PLY: [%3d%%]", (k * 100) / h.face_count);
			fflush(stdout);
		}
		if (dptr >= mapped + st.st_size)
			break ;
		unsigned char	count = *(unsigned char *)dptr;
		dptr++;
		int	idx[4];
		idx[0] = 0;
		idx[1] = 0;
		idx[2] = 0;
		idx[3] = 0;
		int	fi = 0;
		while (fi < (int)count && fi < 4)
		{
			if (dptr + 4 > mapped + st.st_size)
				break ;
			ft_memcpy(&idx[fi], dptr, 4);
			dptr += 4;
			fi++;
		}
		if (idx[0] < 0 || idx[0] >= h.vertex_count
			|| idx[1] < 0 || idx[1] >= h.vertex_count
			|| idx[2] < 0 || idx[2] >= h.vertex_count)
		{
			k++;
			continue ;
		}
		new = ft_calloc(1, sizeof(t_obj));
		if (!new)
			clean_exit(data, 1, "Malloc\n", 0);
		new->tri.p1 = verts[idx[0]];
		new->tri.p2 = verts[idx[1]];
		new->tri.p3 = verts[idx[2]];
		if (h.has_nx >= 0)
			new->tri.normal = norms[idx[0]];
		else
			new->tri.normal = vec_normalize(vec_cross(
						vec_sub(new->tri.p2, new->tri.p1),
						vec_sub(new->tri.p3, new->tri.p1)));
		new->type = CALC_TR;
		new->color = colors[idx[0]];
		new->has_texture = vo.has_tex;
		if (vo.has_tex)
			new->tex = vo.tex;
		if (h.has_s >= 0)
		{
			new->tri.uv1 = uvs[idx[0]];
			new->tri.uv2 = uvs[idx[1]];
			new->tri.uv3 = uvs[idx[2]];
		}
		new->reflectivity = vo.t.reflectivity;
		new->rought = vo.t.rought;
		new->ka = (t_vec3){0.2, 0.2, 0.2};
		new->ks = (t_vec3){1, 1, 1};
		new->kd = (t_vec3){0.8, 0.8, 0.8};
		new->ns = 32;
		new->next = data->objs;
		data->objs = new;
		if (count == 4)
		{
			new = ft_calloc(1, sizeof(t_obj));
			if (!new)
				clean_exit(data, 1, "Malloc\n", 0);
			new->tri.p1 = verts[idx[0]];
			new->tri.p2 = verts[idx[2]];
			new->tri.p3 = verts[idx[3]];
			if (h.has_nx >= 0)
				new->tri.normal = norms[idx[0]];
			else
				new->tri.normal = vec_normalize(vec_cross(
							vec_sub(new->tri.p2, new->tri.p1),
							vec_sub(new->tri.p3, new->tri.p1)));
			new->type = CALC_TR;
			new->color = colors[idx[0]];
			new->has_texture = vo.has_tex;
			if (vo.has_tex)
				new->tex = vo.tex;
			if (h.has_s >= 0)
			{
				new->tri.uv1 = uvs[idx[0]];
				new->tri.uv2 = uvs[idx[2]];
				new->tri.uv3 = uvs[idx[3]];
			}
			new->reflectivity = vo.t.reflectivity;
			new->rought = vo.t.rought;
			new->ka = (t_vec3){0.2, 0.2, 0.2};
			new->ks = (t_vec3){1, 1, 1};
			new->kd = (t_vec3){0.8, 0.8, 0.8};
			new->ns = 32;
			new->opacity = 1;
			new->next = data->objs;
			data->objs = new;
		}
		k++;
	}
	printf("\rParsing PLY: [100%%] - OK.\n");
	munmap(mapped, st.st_size);
	free(verts);
	free(norms);
	free(colors);
	free(uvs);
}
