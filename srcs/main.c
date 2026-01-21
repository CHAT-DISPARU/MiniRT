/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:41:49 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/21 15:39:11 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

/*
	​Rx ​Ux Fx ​Tx​​
	Ry Uy Fy Ty​​
	Rz Uz ​Fz Tz​​
	0  0  0​  1​​
*/

t_mat4	look_at(t_vec3 o, t_vec3 dir)
{
	t_vec3	f = vec_normalize(vec_scale(dir, -1));
	t_vec3	r;
	t_vec3	up_guide = {0, 1, 0};
	t_vec3	u;
	t_mat4	m;
	if (fabs(vec_dot_scal(f, up_guide)) > 0.99)
		up_guide = (t_vec3){1, 0, 0};
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

void	clean_exit(t_data *data, int exit_code)
{
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

/*
	rayon = t*D + O

	plan : y = 0


	t*D + O = 0

	t*D = -O
	t = -O /D
*/

bool	hit_plane(t_plane *pl, t_ray ray, t_hit_r *rec)
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

bool	hit_sphere(t_sphere *sp, t_ray ray, t_hit_r *rec)
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

bool	hit_someting(t_data *data, t_ray ray, t_hit_r *rec, t_plane *floor)
{
	bool	hit;
	double	closest;
	t_hit_r	tmp_rec;

	hit = false;
	closest = INFINITY;
	if (hit_sphere(&data->test_sphere, ray, &tmp_rec))
	{
		if (tmp_rec.t < closest)
		{
			hit = true;
			*rec = tmp_rec;
			closest = tmp_rec.t;
		}
	}
	if (hit_plane(floor, ray, rec))
	{
		if (tmp_rec.t < closest)
		{
			hit = true;
			*rec = tmp_rec;
			closest = tmp_rec.t;
		}
	}
	return (hit);
}

/*
	formule fov equation
	Height = 2×tan(2/FOV​) × FocalLength
*/
void	render(t_data *data)
{
	int			y;
	int			x;
	double		u;
	double		v;
	t_ray		ray;
	t_vec3      local_dir;
	int			idx;
	t_hit_r		rec;
	// ecran de c mort 
	double		fov_radians = data->cam.fov * (PI / 180.0);
	double		focal_length = 1.0;
	double		viewport_height = 2.0 * tan(fov_radians / 2.0) * focal_length;
	double		aspect_ratio = (double)data->width / (double)data->height;
	double		viewport_width = aspect_ratio * viewport_height;
	t_mat4	trans_mtx;
	t_mat4	scale_mtx;
	t_mat4	sphere_matrix;
	t_mat4	trans_plane;
	t_plane	floor;
	t_mat4	cam_matrix = look_at(data->cam.origin, data->cam.dir);

	mat4_initial(&trans_mtx);
	mat4_translation(&trans_mtx, (t_vec3){0, 0, -10}); //pos
	mat4_initial(&scale_mtx);
	mat4_scal(&scale_mtx, (t_vec3){2, 2, 2}); // Rayon de 2
	mat4_initial(&trans_plane);
	mat4_translation(&trans_plane, (t_vec3){0, -2, 0});
	floor.transform = trans_plane;
	floor.inverse_transform = mat4_inverse(&trans_plane);
	sphere_matrix = mat4_mult(&trans_mtx, &scale_mtx);
	data->test_sphere.inverse_transform = mat4_inverse(&sphere_matrix);
	data->test_sphere.transform = sphere_matrix;
	mlx_clear_window(data->mlx, data->win, (mlx_color){.rgba = 0xFF000000});
	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			u = (double)x / (data->width - 1);
			v = (double)y / (data->height - 1);
			local_dir.x = (u * viewport_width) - (viewport_width / 2);
			local_dir.y = (viewport_height / 2) - (v * viewport_height);
			local_dir.z = -focal_length;
			ray.origin = mat4_mult_vec3(&cam_matrix, (t_vec3){0,0,0}, 1.0);
			ray.dir = mat4_mult_vec3(&cam_matrix, local_dir, 0.0);
			ray.dir = vec_normalize(ray.dir);
			idx = y * data->width + x;
			if (hit_someting(data, ray, &rec, &floor))
			{
				double r = (rec.normal.x + 1.0) * 0.5;
   				double g = (rec.normal.y + 1.0) * 0.5;
				double b = (rec.normal.z + 1.0) * 0.5;
				int ir = (int)(255.99 * r);
				int ig = (int)(255.99 * g);
				int ib = (int)(255.99 * b);
				data->pixels[idx].rgba = (ir << 24) | (ig << 16) | (ib << 8) | 0xFF;
			}
			else
				data->pixels[idx].rgba = 0x000000FF;
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

	// protection gimbal lock
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

	data = (t_data *)param;
	// rotaton
	forward = data->cam.dir;
	right = get_right_vector(forward);
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
		render(data);
	}
	if (data->key_table[81])
	{
		mat4_rotate_axis(&rot_mtx, right, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		render(data);
	}
	if (data->key_table[80])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		render(data);
	}
	if (data->key_table[79])
	{
		mat4_rotate_axis(&rot_mtx, (t_vec3){0, 1, 0}, -data->rot_speed);
		data->cam.dir = mat4_mult_vec3(&rot_mtx, data->cam.dir, 0.0);
		render(data);
	}
	if (data->key_table[82] || data->key_table[81] || 
		data->key_table[80] || data->key_table[79])
		data->cam.dir = vec_normalize(data->cam.dir);
	//mouvemnt
	if (data->key_table[26])
	{
		data->cam.origin = vec_add(data->cam.origin, vec_scale(forward, data->speed));
		render(data);
	}
	if (data->key_table[22]) 
	{
		data->cam.origin = vec_sub(data->cam.origin, vec_scale(forward, data->speed));
		render(data);
	}
	if (data->key_table[4]) 
	{
		data->cam.origin = vec_sub(data->cam.origin, vec_scale(right, data->speed));
		render(data);
	}
	if (data->key_table[7]) 
	{
		data->cam.origin = vec_add(data->cam.origin, vec_scale(right, data->speed));
		render(data);
	}
	if (data->key_table[44])
	{
		data->cam.origin.y += data->speed;
		render(data);
	}
	if (data->key_table[225])
	{
		data->cam.origin.y -= data->speed;
		render(data);
	}
	/*
	if (data->key_table[46])
	{
		data->cam.fov -= 2;
		if (data->cam.fov < 1)
			data->cam.fov = 1;
		render(data);
	}
	if (data->key_table[45])
	{
		data->cam.fov += 2;
		if (data->cam.fov > 179)
			data->cam.fov = 179;
		render(data);
	}*/
	ft_memcpy(data->old_key_table, data->key_table, sizeof(data->key_table));
}

int	main(void)
{
	t_data					*data;
	mlx_window_create_info	info;

	ft_memset(&info, 0, sizeof(mlx_window_create_info));
	data = ft_calloc(sizeof(t_data), 1);
	if (!data)
	{
		perror("Malloc fail");
		return (1);
	}
	data->width = WIDTH;
	data->height = HEIGHT;
	data->is_full = false;
	srand(time(NULL));
	ft_memset(data->key_table, 0, sizeof(data->key_table));
	data->cam.origin = (t_vec3){0, 0, 5};
	data->cam.dir = (t_vec3){0, 0, -1};
	data->cam.fov = 70;
	data->speed = 0.5;
	data->rot_speed = 0.05;
	data->mlx = mlx_init();
	if (!data->mlx)
		clean_exit(data, 1);
	info.title = "MiniRT";
	info.width = WIDTH;
	info.height = HEIGHT;
	info.is_resizable = true;
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		clean_exit(data, 1);
	if (resize_win(data) != 0)
		clean_exit(data, 1);
	mlx_set_fps_goal(data->mlx, 60);
	mlx_on_event(data->mlx, data->win, MLX_KEYDOWN, key_down, data);
	mlx_on_event(data->mlx, data->win, MLX_KEYUP, key_up, data);
	mlx_on_event(data->mlx, data->win, MLX_WINDOW_EVENT, window_hook, data);
	mlx_add_loop_hook(data->mlx, update, data);
	mlx_loop(data->mlx);
	clean_exit(data, 0);
	return (0);
}
