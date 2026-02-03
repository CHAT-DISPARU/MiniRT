/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:42:01 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/03 17:10:32 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

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
# include <pthread.h>

# define WIDTH			1500
# define HEIGHT			1000
# define S_PER_PIXS		10
# define KA				1
# define KS				1
# define KD				1
# define PI				3.14159265358979323846
# define THREADS_COUNT	16
# define EPSILON		1e-4

typedef struct s_render_v
{
	double		u;
	double		v;
	double		inv_width;
	double		inv_height;
	t_mat4		cam_m;
	t_vec3		cam_origin;
}				t_render_v;

typedef struct s_face_c
{
	t_vec3			point;
	struct s_face_c	*next;
}				t_face_c;

typedef struct s_ic
{
	int	ir;
	int	ig;
	int	ib;
}				t_ic;

typedef struct s_idxs
{
	int	y;
	int	x;
	int	s;
}				t_idxs;

typedef struct s_color_c
{
	double		obj_r;
	double		obj_g;
	double		obj_b;
	t_vec3		ambient;
	t_vec3		diffuse;
}				t_color_c;

typedef enum e_func
{
	CALC_SP,
	CALC_SQ,
	CALC_PL,
	CALC_CY,
	CALC_TR,
	CALC_HY,
	FLAG_MAX,
}				t_func;

typedef struct s_cy_utils
{
	double	closest_t;
	int		hit_zone;
}				t_cy_utils;

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
	double		t;
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

typedef struct s_triangle
{
	t_vec3	p1;
	t_vec3	p2;
	t_vec3	p3;
}				t_triangle;

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
	double		diameter2;
	mlx_color	col;
	t_triangle	tri;
}				t_mat_t;

typedef struct s_obj
{
	int				type;
	t_triangle		tri;
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
	struct s_light	*next;
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
	int				step;
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
	t_light			*light;
	t_alight		alight;
	double			speed;
	double			rot_speed;
	int				s_per_pixs;
	t_view_p		view_port;
	int				scene_fd;
	char			*scene_line;
}				t_data;

typedef struct s_thread_info
{
	t_data	*data;
	int		start_y;
	int		end_y;
	int		start_x;
	int		end_x;
}				t_thread_info;

typedef struct s_file_info
{
	int		fd;
	char	*line_o;
}				t_file_info;

typedef bool	(*t_calc_f)(t_obj *obj, t_ray ray, t_hit_r *rec);
void		thread_calls(t_data *data);
int			resize_win(t_data *data);
void		*render(void *arg);
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
void		final_diffuse(t_color_c *lights,
				t_light *light, double diff_strength);
void		calc_lights(t_color_c *lights, t_hit_r rec,
				t_data *data, t_light *light);
void		update_rot(t_data *data, t_vec3 right, bool *movded);
bool		hit_triangle(t_obj *tr, t_ray ray, t_hit_r *rec);
void		set_tr(t_data *data, char *line, int i);
void		ft_lightadd_back(t_light **lst, t_light *new);
void		ft_faceadd_back(t_face_c **lst, t_face_c *new);
int			ft_facesize(t_face_c *lst);
void		set_o(t_data *data, char *line, int i);
bool		hit_hyperboloid(t_obj *hy, t_ray ray, t_hit_r *rec);
void		set_hy(t_data *data, char *line, int i);

#endif
