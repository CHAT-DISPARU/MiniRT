/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:41:49 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/31 18:06:10 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

void	ft_objadd_back(t_obj **lst, t_obj *new)
{
	t_obj	*current;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	clean_exit(t_data *data, int exit_code, char *mess_eror, int i)
{
	t_obj	*tmp;

	if (data->scene_fd != -1)
	{
		get_next_line(data->scene_fd, 1);
		close(data->scene_fd);
	}
	if (data->scene_line)
	{
		free(data->scene_line);
		data->scene_line = NULL;
	}
	if (exit_code != EXIT_SUCCESS)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd(mess_eror, 2);
	}
	if (i > 0)
	{
		ft_putstr_fd("line : ", 2);
		ft_putstr_fd(ft_itoa(i), 2);
		ft_putstr_fd("\n", 2);
	}
	while (data->objs)
	{
		tmp = data->objs->next;
		free(data->objs);
		data->objs = tmp;
	}
	if (!data)
		exit(exit_code);
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_context(data->mlx);
	if (data->pixels)
		free(data->pixels);
	free(data);
	exit(exit_code);
}

void	set_a(t_data *data, char *line, int i)
{
	char	*line2;

	if (data->ambient_is_set)
		clean_exit(data, 1, "Error: Multiple Ambient lights\n", i);
	line2 = line + 1;
	check_missing_info(data, line2, i);
	data->a_ratio = rt_atod(&line2);
	check_ratio(data, data->a_ratio, i);
	check_missing_info(data, line2, i);
	data->a_color = parse_color(&line2, data, i);
	check_extra_info(data, line2, i);
	data->ambient_is_set = true;
}

void	set_l(t_data *data, char *line, int i)
{
	char	*line2;
	if (data->light_is_set)
		clean_exit(data, 1, "Error: Multi Light\n", i);
	line2 = line + 1;
	check_missing_info(data, line2, i);
	data->light.origin = parse_vec3(&line2, data, i);
	check_missing_info(data, line2, i);
	data->light.ratio = rt_atod(&line2);
	check_ratio(data, data->light.ratio, i);
	check_missing_info(data, line2, i);
	data->light.color = parse_color(&line2, data, i);
	check_extra_info(data, line2, i);
	data->light_is_set = true;
}

void	set_sq(t_data *data, char *line, int i)
{
	t_obj		*new_sq;
	t_mat4		trans;
	t_mat4		scale;
	t_mat4		rot;
	t_mat4		final;
	t_vec3		center;
	t_vec3		rot_vec;
	double		width;
	double		height;
	mlx_color	col;

	check_missing_info(data, line, i);
	center = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	rot_vec = parse_vec3(&line, data, i);
	check_norm_vec(data, rot_vec, i);
	check_missing_info(data, line, i);
	width = rt_atod(&line);
	check_positive(data, width, i);
	check_missing_info(data, line, i);
	height = rt_atod(&line);
	check_positive(data, height, i);
	check_missing_info(data, line, i);
	col = parse_color(&line, data, i);
	check_extra_info(data, line, i);
	new_sq = malloc(sizeof(t_obj));
	if (!new_sq)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_sq->type = CALC_SQ;
	new_sq->color = col;
	mat4_initial(&trans);
	mat4_initial(&scale);
	mat4_scal(&scale, (t_vec3){width / 2.0, 1.0, height / 2.0});
	rot = mat4_align_vectors((t_vec3){0, 1, 0}, vec_normalize(rot_vec));
	mat4_translation(&trans, center);
	final = mat4_mult(&rot, &scale);
	final = mat4_mult(&trans, &final);
	new_sq->transform = final;
	new_sq->inverse_transform = mat4_inverse(&final);
	new_sq->next = NULL;
	ft_objadd_back(&data->objs, new_sq);
}

void	set_cy(t_data *data, char *line, int i)
{
	t_obj	*new_cy;
	t_mat4	trans;
	t_mat4	scale;
	t_mat4	rot;
	t_mat4	final;
	t_vec3	center;
	t_vec3	rot_vec;
	double	diameter;
	double	height;
	mlx_color	col;

	check_missing_info(data, line, i);
	center = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	rot_vec = parse_vec3(&line, data, i);
	check_norm_vec(data, rot_vec, i);
	check_missing_info(data, line, i);
	diameter = rt_atod(&line);
	check_positive(data, diameter, i);
	check_missing_info(data, line, i);
	height = rt_atod(&line);
	check_positive(data, height, i);
	check_missing_info(data, line, i);
	col = parse_color(&line, data, i);
	check_extra_info(data, line, i);
	new_cy = malloc(sizeof(t_obj));
	if (!new_cy)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_cy->type = CALC_CY;
	new_cy->color = col;
	mat4_initial(&trans);
	mat4_initial(&scale);
	mat4_scal(&scale, (t_vec3){height / 2.0, diameter / 2.0, diameter / 2.0});
	mat4_translation(&trans, center);
	rot = mat4_align_vectors((t_vec3){1, 0, 0}, vec_normalize(rot_vec));
	final = mat4_mult(&rot, &scale);
	final = mat4_mult(&trans, &final);
	new_cy->transform = final;
	new_cy->inverse_transform = mat4_inverse(&final);
	new_cy->next = NULL;
	ft_objadd_back(&data->objs, new_cy);
}

void	set_sp(t_data *data, char *line, int i)
{
	t_obj	*new_sp;
	t_mat4	trans;
	t_mat4	scale;
	t_mat4	final;
	t_vec3	center;
	double	diameter;
	mlx_color	col;

	check_missing_info(data, line, i);
	center = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	diameter = rt_atod(&line);
	check_positive(data, diameter, i);
	check_missing_info(data, line, i);
	col = parse_color(&line, data, i);
	check_extra_info(data, line, i);
	new_sp = malloc(sizeof(t_obj));
	if (!new_sp)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_sp->type = CALC_SP;
	new_sp->color = col;
	mat4_initial(&scale);
	mat4_initial(&trans);
	mat4_scal(&scale, (t_vec3){diameter, diameter, diameter});
	mat4_translation(&trans, center);
	final = mat4_mult(&trans, &scale);
	new_sp->transform = final;
	new_sp->inverse_transform = mat4_inverse(&final);
	new_sp->next = NULL;
	ft_objadd_back(&data->objs, new_sp);
}

void	set_pl(t_data *data, char *line, int i)
{
	t_obj	*new_pl;
	t_mat4	trans;
	t_mat4	rot;
	t_mat4	final;
	t_vec3	center;
	t_vec3	rot_vec;
	mlx_color	col;

	check_missing_info(data, line, i);
	center = parse_vec3(&line, data, i);
	check_missing_info(data, line, i);
	rot_vec = parse_vec3(&line, data, i);
	check_norm_vec(data, rot_vec, i);
	check_missing_info(data, line, i);
	col = parse_color(&line, data, i);
	check_extra_info(data, line, i);
	new_pl = malloc(sizeof(t_obj));
	if (!new_pl)
		clean_exit(data, 1, "malloc fail\n", 0);
	new_pl->type = CALC_PL;
	new_pl->color = col;
	mat4_initial(&trans);
	mat4_translation(&trans, center);
	rot = mat4_align_vectors((t_vec3){0, 1, 0}, vec_normalize(rot_vec));
	final = mat4_mult(&trans, &rot);
	new_pl->transform = final;
	new_pl->inverse_transform = mat4_inverse(&final);
	new_pl->next = NULL;
	ft_objadd_back(&data->objs, new_pl);
}

void	set_c(t_data *data, char *line, int i)
{
	t_vec3	origin;
	t_vec3	dir;
	double	fov;
	char	*line2;

	line2 = line + 1;
	if (data->camera_is_set == true)
		clean_exit(data, 1, "Error: Multiple cameras defined\n", i);
	check_missing_info(data, line2, i);
	origin = parse_vec3(&line2, data, i);
	check_missing_info(data, line2, i);
	dir = parse_vec3(&line2, data, i);
	check_norm_vec(data, dir, i);
	check_missing_info(data, line2, i);
	fov = rt_atod(&line2);
	check_fov(data, fov, i);
	check_extra_info(data, line2, i);
	data->cam.origin = origin;
	data->cam.dir = vec_normalize(dir);
	data->cam.fov = fov;
	data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
	data->view_port.focal_length = 1.0;
	data->view_port.viewport_height = 2.0 * tan(data->view_port.fov_radians / 2.0) * data->view_port.focal_length;
	data->view_port.aspect_ratio = (double)data->width / (double)data->height;
	data->view_port.viewport_width = data->view_port.aspect_ratio * data->view_port.viewport_height;
	data->camera_is_set = true;
}

void	read_rt(t_data *data, char *filename)
{
	int		i;
	char	buffer[1];
	char	*ptr;

	i = 1;
	data->scene_fd = open(filename, O_RDONLY);
	if (data->scene_fd < 0)
		clean_exit(data, 1, "open fail\n", 0);
	if (read(data->scene_fd, buffer, 0) == -1)
		clean_exit(data, 1, "Error: The file is a directory\n", 0);
	data->scene_line = get_next_line(data->scene_fd, 0);
	while (data->scene_line)
	{
		ptr = data->scene_line;
		while (*ptr && is_space(*ptr))
			ptr++;
		if (*ptr && *ptr != '\n')
		{
			if (*ptr == 'A' && is_space(ptr[1]))
				set_a(data, ptr, i);
			else if (*ptr == 'C' && is_space(ptr[1]))
				set_c(data, ptr, i);
			else if (*ptr == 'L' && is_space(ptr[1]))
				set_l(data, ptr, i);
			else if (!ft_strncmp("sp", ptr, 2) && is_space(ptr[2]))
				set_sp(data, ptr + 2, i);
			else if (!ft_strncmp("pl", ptr, 2) && is_space(ptr[2]))
				set_pl(data, ptr + 2, i);
			else if (!ft_strncmp("cy", ptr, 2) && is_space(ptr[2]))
				set_cy(data, ptr + 2, i);
			else if (!ft_strncmp("sq", ptr, 2) && is_space(ptr[2]))
				set_sq(data, ptr + 2, i);
			else
				clean_exit(data, EXIT_FAILURE, "Wrong identifier\n", i);
		}
		free(data->scene_line);
		data->scene_line = get_next_line(data->scene_fd, 0);
		i++;
	}
	close(data->scene_fd);
	data->scene_fd = -1;
}

double	rand_double(void)
{
	return ((double)rand() / (double)RAND_MAX);
}

/*
	​Rx ​Ux Fx ​Tx​​
	Ry Uy Fy Ty​​
	Rz Uz ​Fz Tz​​
	0  0  0​  1​​
*/

t_mat4	look_at(t_vec3 o, t_vec3 dir, t_vec3 up_guide)
{
	t_vec3	f = vec_normalize(vec_scale(dir, -1));
	t_vec3	r;
	t_vec3	u;
	t_mat4	m;

	if (fabs(vec_dot_scal(f, up_guide)) > 0.99)
		up_guide = vec_add(up_guide, (t_vec3){0.1, 0, 0});
	r = vec_normalize(vec_cross(up_guide, f));
	u = vec_cross(f, r);
	mat4_initial(&m);
	m.m[0][0] = r.x;
	m.m[0][1] = u.x;
	m.m[0][2] = f.x;
	m.m[0][3] = o.x;
	m.m[1][0] = r.y;
	m.m[1][1] = u.y;
	m.m[1][2] = f.y;
	m.m[1][3] = o.y;
	m.m[2][0] = r.z;
	m.m[2][1] = u.z;
	m.m[2][2] = f.z;
	m.m[2][3] = o.z;
	m.m[3][0] = 0;
	m.m[3][1] = 0;
	m.m[3][2] = 0;
	m.m[3][3] = 1;
	return (m);
}

void	key_up(int key, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (key >= 0 && key < 512)
		data->key_table[key] = 0;
}

void	key_down(int key, void *param)
{
	t_data	*data;

	printf("%d\n", key);
	data = (t_data *)param;
	if (key == 41)
		mlx_loop_end(data->mlx);
	if (key >= 0 && key < 512)
		data->key_table[key] = 1;
}

void	window_hook(int event, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (event == 0)
		mlx_loop_end(data->mlx);
	else if (event == 1)
	{
		if (resize_win(data) != 0)
			mlx_loop_end(data->mlx);
	}
}

bool	hit_border_cy(t_ray ray, double x_plane, double *t_out)
{
	double	t;
	double	y;
	double	z;

	if (fabs(ray.dir.x) < 0.0001)
		return (false);
	t = (x_plane - ray.origin.x) / ray.dir.x;
	if (t < 0.001)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	z = ray.origin.z + t * ray.dir.z;
	if ((y * y + z * z) > 1.0)
		return (false);
	*t_out = t;
	return (true);
}

/*
	x(t) = d0t + o0
	y(t) = d1t + o1
	z(t) = d2t + o2


	r(t) = O + TD

	y² + z² = r²

	D(d0, d1, d2)
	O(o0, o1, o2)

	(d1t + O1)² + (d2t + o2)²= r²
	d1t² + 2(d1t * o1) + o1² + d2t² + 2(d2t * o2) + o2² - r² = 0
	t²(d1 + d2) + 2t(d1 * o1 + d2 * o2) + 2o2² - r²
*/

bool	hit_cylinder(t_obj *cy, t_ray ray, t_hit_r *rec)
{
	t_ray	l_ray;
	double	a;
	double	b;
	double	c;
	double	closest_t = INFINITY;
	int		hit_zone = 0;
	double	delta;
	double	t;

	l_ray.origin = mat4_mult_vec3(&cy->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&cy->inverse_transform, ray.dir, 0.0);
	a = (l_ray.dir.y * l_ray.dir.y) + (l_ray.dir.z * l_ray.dir.z);
	b = ((l_ray.origin.y * l_ray.dir.y) + (l_ray.origin.z * l_ray.dir.z));
	c = (l_ray.origin.y * l_ray.origin.y) + (l_ray.origin.z * l_ray.origin.z) - 1.0;
	delta = (b * b) - (a * c);
	if (delta >= 0)
	{
		t = (-b - sqrt(delta)) / (a);
		if (t > 0.001)
		{
			double hit_x = l_ray.origin.x + t * l_ray.dir.x;
			if (hit_x >= -1.0 && hit_x <= 1.0)
			{
				closest_t = t;
				hit_zone = 1;
			}
		}
		if (hit_zone == 0)
		{
			t = (-b + sqrt(delta)) / a;
			if (t > 0.001)
			{
				double hit_x = l_ray.origin.x + t * l_ray.dir.x;
				if (hit_x >= -1.0 && hit_x <= 1.0)
				{
					closest_t = t;
					hit_zone = 1;
				}
			}
		}
	}
	if (hit_border_cy(l_ray, 1.0, &t))
	{
		if (t < closest_t)
		{
			hit_zone = 2;
			closest_t = t;
		}
	}
	if (hit_border_cy(l_ray, -1.0, &t))
	{
		if (t < closest_t)
		{
			hit_zone = 3;
			closest_t = t;
		}
	}
	if (hit_zone == 0)
		return (false);
	rec->t = closest_t;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, closest_t));
	t_vec3 local_normal;
	if (hit_zone == 1)
	{
		local_normal = vec_add(l_ray.origin, vec_scale(l_ray.dir, closest_t));
		local_normal.x = 0;
	}
	else if (hit_zone == 2)
		local_normal = (t_vec3){1, 0, 0};
	else
		local_normal = (t_vec3){-1, 0, 0};
	rec->normal = mat4_mult_vec3(&cy->transform, local_normal, 0.0);
	rec->normal = vec_normalize(rec->normal);
	return (true);
}

bool	hit_square(t_obj *sq, t_ray ray, t_hit_r *rec)
{
	t_ray	l_ray;
	double	t;
	double	denom;
	t_vec3	p;

	l_ray.origin = mat4_mult_vec3(&sq->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&sq->inverse_transform, ray.dir, 0.0);
	denom = l_ray.dir.y;
	if (fabs(denom) < 0.00001)
		return (false);
	t = -l_ray.origin.y / denom;
	if (t < 0.001)
		return (false);
	p = vec_add(l_ray.origin, vec_scale(l_ray.dir, t));
	if (fabs(p.x) > 1.0 || fabs(p.z) > 1.0)
		return (false);
	rec->t = t;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, t));
	t_vec3 local_normal = {0, 1, 0};
	rec->normal = mat4_mult_vec3(&sq->transform, local_normal, 0.0);
	rec->normal = vec_normalize(rec->normal);
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	return (true);
}

/*
	rayon = t*D + O

	plan : y = 0


	t*D + O = 0

	t*D = -O
	t = -O /D
*/

bool	hit_plane(t_obj *pl, t_ray ray, t_hit_r *rec)
{
	t_ray	l_ray;
	double	t;
	double	denom;

	l_ray.origin = mat4_mult_vec3(&pl->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&pl->inverse_transform, ray.dir, 0.0);
	denom = l_ray.dir.y;
	if (fabs(denom) < 0.00001)
		return (false);
	t = -l_ray.origin.y / denom;
	if (t < 0.001)
		return (false);
	rec->t = t;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, t));
	t_vec3 local_normal = {0, 1, 0};
	rec->normal = mat4_mult_vec3(&pl->transform, local_normal, 0.0);
	rec->normal = vec_normalize(rec->normal);
	if (vec_dot_scal(ray.dir, rec->normal) > 0)
		rec->normal = vec_scale(rec->normal, -1.0);
	return (true);
}

/*
	D direction O origine t inconue

	Ray(t) = o + t * D;

	tous les point f d'une sphere

	||f - c|| = r²; longeur vecteur au carre est strictement egual au produit scalire de lui meme
	(f - c) * (f - c) = r²
	((O + t * D) - C) * ((O + t * D) - C) = r²
	(OC + TD) * (OC + TD) = r²

	(a + b)² = a² + 2ab + b²
	t = -l_ray.origin.y / denom;

	(OC * OC) + 2(OC * TD) + TD² = r²
	(OC * OC) + 2t(OC * D) + t²(D * D) = r²
	(OC * OC) + 2t(OC * D) + t²(D * D) - r² = 0
*/

bool	hit_sphere(t_obj *sp, t_ray ray, t_hit_r *rec)
{
	t_ray	l_ray;
	double  a;
	double	b;
	double	c;
	double	delta;

	l_ray.origin = mat4_mult_vec3(&sp->inverse_transform, ray.origin, 1.0);
	l_ray.dir = mat4_mult_vec3(&sp->inverse_transform, ray.dir, 0.0);
	a = vec_dot_scal(l_ray.dir, l_ray.dir);
	b = vec_dot_scal(l_ray.origin, l_ray.dir);
	c = vec_dot_scal(l_ray.origin, l_ray.origin) - 1.0;
	delta = (b * b) - (a * c);
	if (delta < 0)
		return (false);
	double t = (-b - sqrt(delta)) / (a);
	if (t < 0.001)
	{
		t = (-b + sqrt(delta)) / (a);
		if (t < 0.001)
			return (false);
	}
	rec->t = t;
	rec->p = vec_add(ray.origin, vec_scale(ray.dir, t));
	t_vec3 local_hit = vec_add(l_ray.origin, vec_scale(l_ray.dir, t));
	t_vec3 local_normal = local_hit; 
	rec->normal = mat4_mult_vec3(&sp->transform, local_normal, 0);
	rec->normal = vec_normalize(rec->normal);
	return (true);
}

void	init_t_calc_f(t_calc_f *functions)
{
	functions[CALC_SQ] = hit_square;
	functions[CALC_CY] = hit_cylinder;
	functions[CALC_PL] = hit_plane;
	functions[CALC_SP] = hit_sphere;
}

bool	hit_someting(t_data *data, t_ray ray, t_hit_r *rec)
{
	bool	hit;
	double	closest;
	t_obj	*objs;
	t_hit_r	tmp_rec;
	static t_calc_f functions[FLAG_MAX];
	static bool		is_init = false;

	if (!is_init)
	{
		init_t_calc_f(functions);
		is_init = true;
	}
	hit = false;
	objs = data->objs;
	closest = INFINITY;
	while (objs)
	{
		if (functions[objs->type](objs, ray, &tmp_rec))
		{
			if (tmp_rec.t < closest)
			{
				hit = true;
				*rec = tmp_rec;
				rec->color = objs->color;
				closest = tmp_rec.t;
			}
		}
		objs = objs->next;
	}
	return (hit);
}

/*
	formule fov equation
	Height = 2×tan(2/FOV​) × FocalLength

	Couleur Amb ​= Couleur Obj ​× (Couleur Amb​ × Ratio Amb​)
	Couleur Diff​ = Couleur Obj ​× (Couleur Light ​× Ratio Light​)
		× (Normal ⋅ Light Dir)
*/
void	render(t_data *data)
{
	int			y;
	int			x;
	int			s;
	double		u;
	double		v;
	t_ray		ray;
	t_vec3      local_dir;
	int			idx;
	t_hit_r		rec;
	t_vec3		color_acc;
	t_vec3		pixel;
	t_vec3		final_color;
	double		inv_width = 1.0 / (data->width - 1);
	double		inv_height = 1.0 / (data->height - 1);

	ft_bzero(&rec, sizeof (t_hit_r));
	t_mat4	cam_matrix = look_at(data->cam.origin, data->cam.dir, data->cam.up_guide);
	t_vec3	cam_origin = mat4_mult_vec3(&cam_matrix, (t_vec3){0,0,0}, 1.0);
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			color_acc = (t_vec3){0, 0, 0};
			s = 0;
			while (s < data->s_per_pixs)
			{
				if (data->s_per_pixs == 1)
				{
					u = ((double)x + 0.5) * inv_width;
					v = ((double)y + 0.5) * inv_height;
				}
				else
				{
					u = ((double)x + rand_double()) * inv_width;
					v = ((double)y + rand_double()) * inv_height;
				}
				local_dir.x = (u * data->view_port.viewport_width) - (data->view_port.viewport_width / 2);
				local_dir.y = (data->view_port.viewport_height / 2) - (v * data->view_port.viewport_height);
				local_dir.z = -data->view_port.focal_length;
				ray.origin = cam_origin;
				ray.dir = mat4_mult_vec3(&cam_matrix, local_dir, 0.0);
				ray.dir = vec_normalize(ray.dir);
				if (hit_someting(data, ray, &rec))
				{
					double	obj_r = rec.color.r / 255.0;
					double	obj_g = rec.color.g / 255.0;
					double	obj_b = rec.color.b / 255.0;
					t_vec3	ambient;
					ambient.x = obj_r * data->a_final.x;
					ambient.y = obj_g * data->a_final.y;
					ambient.z = obj_b * data->a_final.z;
					t_vec3	diffuse;
					diffuse.x = data->light.color.r / 255.0;
					diffuse.y = data->light.color.g / 255.0;
					diffuse.z = data->light.color.b / 255.0;
					if (is_in_shadow(data, &rec, &data->light) == false)
					{
						t_vec3	light_dir = vec_sub(data->light.origin, rec.p);
						light_dir = vec_normalize(light_dir);
						double	diff_strength = vec_dot_scal(rec.normal, light_dir);
						if (diff_strength > 0)
						{
							diffuse.x *= obj_r * data->light.ratio * diff_strength;
							diffuse.y *= obj_g * data->light.ratio * diff_strength;
							diffuse.z *= obj_b * data->light.ratio * diff_strength;
						}
						else
							diffuse = (t_vec3){0, 0, 0};
					}
					else
						diffuse = (t_vec3){0, 0, 0};
					pixel = vec_add(ambient, diffuse);
					color_acc = vec_add(color_acc, pixel);
				}
				s++;
			}
			final_color = vec_scale(color_acc, 1.0 / data->s_per_pixs);
			if (final_color.x > 1.0)
				final_color.x = 1.0;
			if (final_color.y > 1.0)
				final_color.y = 1.0;
			if (final_color.z > 1.0)
				final_color.z = 1.0;
			int ir = (int)(255.999 * final_color.x);
			int ig = (int)(255.999 * final_color.y);
			int ib = (int)(255.999 * final_color.z);
			idx = y * data->width + x;
			data->pixels[idx].rgba = (ir << 24) | (ig << 16) | (ib << 8) | 0xFF;
			x++;
		}
		y++;
	}
	mlx_set_image_region(data->mlx, data->img, 0, 0, data->width, data->height, data->pixels);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int	resize_win(t_data *data)
{
	if (data->img)
	{
		mlx_destroy_image(data->mlx, data->img);
		data->img = NULL;
	}
	if (data->pixels)
	{
		free(data->pixels);
		data->pixels = NULL;
	}
	mlx_get_window_size(data->mlx, data->win, &data->width, &data->height);
	data->img = mlx_new_image(data->mlx, data->width, data->height);
	data->pixels = calloc(data->width * data->height, sizeof(mlx_color));
	if (!data->img || !data->pixels)
		return (1);
	render(data);
	return (0);
}

t_vec3	get_right_vector(t_vec3 dir)
{
	t_vec3	up_guide = {0, 1, 0};
	t_vec3	right;

	/*protection gimbal lock*/
	if (fabs(vec_dot_scal(dir, up_guide)) > 0.99)
		up_guide = (t_vec3){1, 0, 0};
	right = vec_normalize(vec_cross(dir, up_guide));
	return (right);
}

void	update(void *param)
{
	t_data	*data;
	t_mat4	rot_mtx;
	t_vec3	forward;
	t_vec3	right;
	bool	movded;

	data = (t_data *)param;
	movded = false;
	// rotaton
	forward = data->cam.dir;
	right = get_right_vector(forward);
	if (data->key_table[30] && !data->old_key_table[30])
	{
		movded = true;
		data->s_per_pixs = 1;
	}
	if (data->key_table[31] && !data->old_key_table[31])
	{
		movded = true;
		data->s_per_pixs = 3;
	}
	if (data->key_table[32] && !data->old_key_table[32])
	{
		movded = true;
		data->s_per_pixs = 5;
	}
	if (data->key_table[33] && !data->old_key_table[33])
	{
		movded = true;
		data->s_per_pixs = S_PER_PIXS;
	}
	if (data->key_table[224] && !data->old_key_table[224])
	{
		if (data->speed == 0.5)
		{
			data->speed = 2;
			data->rot_speed = 0.2;
		}
		else
		{
			data->speed = 0.5;
			data->rot_speed = 0.05;
		}
	}
	if (data->key_table[82])
	{
		mat4_rotate_axis(&rot_mtx, right, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[81])
	{
		mat4_rotate_axis(&rot_mtx, right, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[80])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[79])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		movded = true;
	}
	if (data->key_table[47])
	{
		mat4_rotate_axis(&rot_mtx, data->cam.dir, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		data->cam.up_guide = mat4_mult_vec3(&rot_mtx, data->cam.up_guide, 0.0);
		data->cam.up_guide = vec_normalize(data->cam.up_guide);
		movded = true;
	}
	if (data->key_table[48])
	{
		mat4_rotate_axis(&rot_mtx, data->cam.dir, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		data->cam.up_guide = mat4_mult_vec3(&rot_mtx, data->cam.up_guide, 0.0);
		data->cam.up_guide = vec_normalize(data->cam.up_guide);
		movded = true;
	}
	if (data->key_table[82] || data->key_table[81] || 
		data->key_table[80] || data->key_table[79])
		data->cam.dir = vec_normalize(data->cam.dir);
	//mouvement
	if (data->key_table[26])
	{
		data->cam.origin = vec_add(data->cam.origin, vec_scale(forward, data->speed));
		movded = true;
	}
	if (data->key_table[22]) 
	{
		data->cam.origin = vec_sub(data->cam.origin, vec_scale(forward, data->speed));
		movded = true;
	}
	if (data->key_table[4]) 
	{
		data->cam.origin = vec_sub(data->cam.origin, vec_scale(right, data->speed));
		movded = true;
	}
	if (data->key_table[7]) 
	{
		data->cam.origin = vec_add(data->cam.origin, vec_scale(right, data->speed));
		movded = true;
	}
	if (data->key_table[44])
	{
		data->cam.origin.y += data->speed;
		movded = true;
	}
	if (data->key_table[225])
	{
		data->cam.origin.y -= data->speed;
		movded = true;
	}
	if (data->key_table[46])
	{
		data->cam.fov -= 2;
		if (data->cam.fov < 1)
			data->cam.fov = 1;
		data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
		data->view_port.viewport_height = 2.0 * tan(data->view_port.fov_radians / 2.0) * data->view_port.focal_length;
		data->view_port.viewport_width = data->view_port.aspect_ratio * data->view_port.viewport_height;
		movded = true;
	}
	if (data->key_table[45])
	{
		data->cam.fov += 2;
		if (data->cam.fov > 179)
			data->cam.fov = 179;
		data->view_port.fov_radians = data->cam.fov * (PI / 180.0);
		data->view_port.viewport_height = 2.0 * tan(data->view_port.fov_radians / 2.0) * data->view_port.focal_length;
		data->view_port.viewport_width = data->view_port.aspect_ratio * data->view_port.viewport_height;
		movded = true;
	}
	if (movded)
	{
		//int old = data->s_per_pixs;

		//data->s_per_pixs = 1;
		render(data);
		//data->s_per_pixs = old;
	}
	ft_memcpy(data->old_key_table, data->key_table, sizeof(data->key_table));
}

void	calcul_ambient(t_data *data)
{
	double	ratio;

	ratio = data->a_ratio;
	data->a_final.x = (data->a_color.r / 255.0) * ratio;
	data->a_final.y = (data->a_color.g / 255.0) * ratio;
	data->a_final.z = (data->a_color.b / 255.0) * ratio;
}

void	init_data(t_data *data, mlx_window_create_info info, char *filename)
{
	data->camera_is_set = false;
	data->ambient_is_set = false;
	data->width = WIDTH;
	data->height = HEIGHT;
	data->is_full = false;
	data->scene_fd = -1;
	data->scene_line = NULL;
	data->s_per_pixs = 1;
	data->objs = NULL;
	srand(time(NULL));
	ft_memset(data->key_table, 0, sizeof(data->key_table));
	data->cam.up_guide = (t_vec3){0, 1, 0};
	data->speed = 0.5;
	data->rot_speed = 0.05;
	read_rt(data, filename);
	calcul_ambient(data);
	data->mlx = mlx_init();
	if (!data->mlx)
		clean_exit(data, 1, "Error init MLX\n", 0);
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		clean_exit(data, 1, "Error Malloc MLX\n", 0);
	if (resize_win(data) != 0)
		clean_exit(data, 1, "Error Malloc MLX\n", 0);
}

int	main(int ac, char **av)
{
	t_data					*data;
	mlx_window_create_info	info;

	if (ac != 2)
	{
		ft_putstr_fd("invalid argument:\n./MiniRT <file.rt>\n", 2);
		return (1);
	}
	ft_memset(&info, 0, sizeof(mlx_window_create_info));
	data = ft_calloc(sizeof(t_data), 1);
	if (!data)
	{
		perror("Malloc fail");
		return (1);
	}
	info.title = "MiniRT";
	info.width = WIDTH;
	info.height = HEIGHT;
	info.is_resizable = true;
	init_data(data, info, av[1]);
	mlx_set_fps_goal(data->mlx, 60);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_down, data);
	mlx_on_event(data->mlx, data->win, MLX_KEYUP, key_up, data);
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook, data);
	mlx_add_loop_hook(data->mlx, update, data);
	mlx_loop(data->mlx);
	clean_exit(data, 0, NULL, 0);
	return (0);
}
