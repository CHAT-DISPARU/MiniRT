/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:42:01 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/31 23:20:12 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "get_next_line.h"
# include "gajanvielib_math.h"
# include "lib_vec.h"
# include "lib_mat.h"
# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <fcntl.h> 
# include <errno.h>
# include <stdlib.h>
# include <mlx.h>
# include <mlx_extended.h>
# include <time.h>

# define WIDTH		1500
# define HEIGHT		1000
# define S_PER_PIXS	10
# define KA			1
# define KS			1
# define KD			1
# define PI			3.14159265358979323846

typedef enum e_func
{
	CALC_SP,
	CALC_SQ,
	CALC_PL,
	CALC_CY,
	FLAG_MAX,
}				t_func;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	dir;
}				t_ray;

typedef struct s_camera
{
	t_vec3	origin;
	t_vec3	dir;
	t_vec3	up_guide;
	double	fov;
}				t_camera;

typedef struct s_hit_r
{
	t_vec3		p;
	t_vec3		normal;
	mlx_color	color;
	double	t;
}				t_hit_r;

typedef struct s_hit_some
{
	bool	hit;
	double	closest;
	t_hit_r	tmp_rec;
}				t_hit_some;

typedef struct s_hit
{
	t_ray	l_ray;
	t_vec3	local_normal;
	double	t;
}				t_hit;

typedef struct s_mat_t
{
	t_mat4		trans;
	t_mat4		scale;
	t_mat4		rot;
	t_mat4		final;
	t_vec3		center;
	t_vec3		rot_vec;
	double		width;
	double		height;
	double		diameter;
	mlx_color	col;
}				t_mat_t;

typedef struct s_obj
{
	int				type;
	t_mat4			transform;
	t_mat4			inverse_transform;
	mlx_color		color;
	struct s_obj	*next;
}				t_obj;

typedef struct s_light
{
	t_vec3			origin;
	double			ratio;
	mlx_color		color;
}				t_light;

typedef struct s_alight
{
	int				type;
	double			ratio;
	mlx_color		color;
}				t_alight;

typedef struct s_view_p
{
	double		fov_radians;
	double		focal_length;
	double		viewport_height;
	double		aspect_ratio;
	double		viewport_width;
}				t_view_p;

typedef struct s_data
{
	bool			camera_is_set;
	bool			ambient_is_set;
	bool			light_is_set;
	int				width;
	int				height;
	mlx_context		mlx;
	mlx_color		a_color;
	double			a_ratio;
	t_vec3			a_final;
	void			*win;
	void			*img;
	mlx_color		*pixels;
	bool			is_full;
	int				key_table[512];
	int				old_key_table[512];
	t_camera		cam;
	t_obj			*objs;
	t_light			light;
	t_alight		alight;
	double			speed;
	double			rot_speed;
	int				s_per_pixs;
	t_view_p		view_port;
	int				scene_fd;
	char			*scene_line;
}				t_data;

typedef bool	(*t_calc_f)(t_obj *obj, t_ray ray, t_hit_r *rec);
int			resize_win(t_data *data);
void		render(t_data *data);
mlx_color	parse_color(char **line, t_data *data, int i);
t_vec3		parse_vec3(char **line, t_data *data, int i);
double		rt_atod(char **line);
void		skip_coma(char **line, t_data *data, int i);
void		skip_spaces(char **line);
t_mat4		mat4_align_vectors(t_vec3 start, t_vec3 dest);
bool		is_in_shadow(t_data *data, t_hit_r *rec, t_light *light);
void		init_t_calc_f(t_calc_f *functions);
void		check_color_val(t_data *data, double c, int i);
void		check_extra_info(t_data *data, char *line, int i);
void		check_norm_vec(t_data *data, t_vec3 v, int i);
void		check_missing_info(t_data *data, char *line, int i);
void		check_ratio(t_data *data, double ratio, int i);
void		check_fov(t_data *data, double fov, int i);
void		check_positive(t_data *data, double val, int i);
void		clean_exit(t_data *data, int exit_code, char *mess_eror, int i);
void		window_hook(int event, void *param);
void		key_down(int key, void *param);
void		key_up(int key, void *param);
bool		hit_sphere(t_obj *sp, t_ray ray, t_hit_r *rec);
bool		hit_plane(t_obj *pl, t_ray ray, t_hit_r *rec);
bool		hit_square(t_obj *sq, t_ray ray, t_hit_r *rec);
bool		hit_cylinder(t_obj *cy, t_ray ray, t_hit_r *rec);
bool		hit_someting(t_data *data, t_ray ray, t_hit_r *rec);
t_mat4		look_at(t_vec3 o, t_vec3 dir, t_vec3 up_guide);
void		set_pl(t_data *data, char *line, int i);
void		read_rt(t_data *data, char *filename);
void		set_c(t_data *data, char *line, int i);
void		set_sp(t_data *data, char *line, int i);
void		set_cy(t_data *data, char *line, int i);
void		set_sq(t_data *data, char *line, int i);
void		set_l(t_data *data, char *line, int i);
void		set_a(t_data *data, char *line, int i);
void		update(void *param);
t_vec3		get_right_vector(t_vec3 dir);
void		calcul_ambient(t_data *data);
double		rand_double(void);
void		ft_objadd_back(t_obj **lst, t_obj *new);

#endif
