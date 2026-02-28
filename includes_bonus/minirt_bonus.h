/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:42:01 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/28 14:37:23 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

# include "libft.h"
# include "get_next_line.h"
# include "gajanvielib_math.h"
# include "lib_vec.h"
# include "lib_mat.h"
# include <stdatomic.h>
# include <stdio.h>
# include <math.h>
# include <sys/time.h>
# include <stdlib.h>
# include <fcntl.h> 
# include <errno.h>
# include <stdlib.h>
# include <mlx.h>
# include <mlx_extended.h>
# include <time.h>
# include <pthread.h>
# include <unistd.h>
# include <float.h>

# define CHUNK_SIZE (64 * 1024 * 1024)
# define WIDTH			1500
# define HEIGHT			1000
# define S_PER_PIXS		100
# define NB_TASK_R		24
# define PI				3.14159265358979323846
# define THREADS_COUNT	16
# define MAX_BVH_DEPTH	24
# define EPSILON		1e-4

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}				t_aabb;

typedef struct s_bvh_node
{
	t_aabb	box;
	int		left;
	int		right;
	int		start_idx;
	int		obj_count;
	int		depth;
	t_vec3	debug_color;
}				t_bvh_node;

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
	CALC_CO,
	FLAG_MAX,
}				t_func;

typedef struct s_cy_utils
{
	double	closest_t;
	int		hit_zone;
	double	half_h;
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

typedef struct s_hit_some
{
	bool	hit;
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
	t_vec3	normal;
	t_vec2		uv1;
	t_vec2		uv2;
	t_vec2		uv3;
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
	double		scaled;
	double		diameter;
	double		diameter2;
	mlx_color	col;
	t_triangle	tri;
	double		reflectivity;
	double		rought;
}				t_mat_t;

typedef struct s_texture
{
	void		*img;
	mlx_color	*pixels;
	int			width;
	int			height;
}				t_texture;

typedef struct s_obj
{
	double			rad_1;
	double			rad_2;
	double			height;
	t_vec3			ka;
	double			ns;
	t_vec3			kd;
	t_vec3			ks;
	int				type;
	t_triangle		tri;
	t_mat4			transform;
	t_mat4			inverse_transform;
	mlx_color		color;
	double			reflectivity;
	double			rought;
	bool			has_texture;
	bool			has_bump;
	t_texture		*tex;
	t_texture		*bump;
	struct s_obj	*next;
}				t_obj;

typedef struct s_mtl_info
{
	t_vec3				ka;
	t_texture			*bump;
	double				ns;
	t_vec3				kd;
	t_vec3				ks;
	t_texture			*tex;
	char				*idx;
	double				reflectivity;
	double				rought;
	mlx_color			color;
	bool				has_col;
	struct s_mtl_info	*next;
}				t_mtl_info;


typedef struct s_hit_r
{
	t_vec3		p;
	t_vec3		normal;
	mlx_color	color;
	double		t;
	double		u;
	double		v;
	t_obj		*obj_ptr;
}				t_hit_r;

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
	t_list					*textures;
	mlx_color				checker_color;
	bool					has_checker;
	bool					diff_ok;
	int						deph;
	double					last_frame_time;
	bool					use_bvh;
	t_mtl_info				*mtl_info;
	int						step;
	bool					camera_is_set;
	bool					ambient_is_set;
	bool					light_is_set;
	int						width;
	int						height;
	mlx_context				mlx;
	mlx_color				a_color;
	double					a_ratio;
	t_vec3					a_final;
	void					*win;
	void					*img;
	mlx_color				*pixels;
	bool					is_full;
	int						key_table[512];
	int						old_key_table[512];
	t_camera				cam;
	int						nodes_used;
	bool					debug;
	int						debug_depth;
	t_obj					*objs;
	t_obj					**sorted_objs;
	t_bvh_node				*bvh_nodes;
	t_obj					*array_obj;
	t_aabb					*obj_aabbs;
	int						obj_count;
	t_obj					*plane_array;
	int						plane_count;
	t_light					*light;
	int						nodes_capacity;
	t_alight				alight;
	double					speed;
	double					rot_speed;
	int						s_per_pixs;
	t_view_p				view_port;
	int						scene_fd;
	char					*scene_line;
	char					*filename;
	pthread_mutex_t			mutex_stack;
	pthread_mutex_t			finish_count;
	_Atomic int						proccessed_objs;
	int						finish;
	bool					thread_running;
	pthread_t				threads[THREADS_COUNT];
	bool					lines;
	int						scale;
	struct s_thread_p		*stack;
}				t_data;

typedef struct s_thread_info
{
	t_data	*data;
	int		start_y;
	int		end_y;
	int		start_x;
	int		end_x;
	int		current_line;
	int		total_lines;
}				t_thread_info;


typedef void	(*t_task)(void *arg);

typedef struct s_thread_p
{
	t_thread_info		info;
	t_task				task;
	struct s_thread_p	*next;
}				t_thread_p;

typedef struct s_file_info
{
	int		fd;
	char	*line_o;
}				t_file_info;

typedef struct s_vars_obj
{
	t_mat_t		t;
	t_obj		*new;
	t_texture	*tex;
	char		*file;
	char		*tex_p;
	char		*str;
	t_vec3		*v;
	t_vec3		*vn;
	t_vec3		*vt;
	int			cts[3];
	long long	idx[3];
	bool		has_tex;
	size_t		len;
	size_t		pos;
	size_t		step;
	size_t		next;
}				t_vars_obj;

typedef enum e_ply_type
{
	PLY_FLOAT,
	PLY_UCHAR,
	PLY_INT,
	PLY_UINT,
	PLY_DOUBLE,
	PLY_SHORT,
	PLY_USHORT,
	PLY_UNKNOWN
}	t_ply_type;

typedef struct s_ply_prop
{
	char		name[64];
	t_ply_type	type;
	int			offset;
	int			size;
}	t_ply_prop;

typedef struct s_ply_header
{
	int			vertex_count;
	int			face_count;
	t_ply_prop	props[16];
	int			prop_count;
	int			vertex_size;
	int			has_nx;
	int			has_r;
	int			has_s;
	int			data_offset;
	bool		is_binary;
}	t_ply_header;

typedef bool	(*t_calc_f)(t_obj *obj, t_ray ray, t_hit_r *rec);
void		thread_calls(t_data *data);
int			resize_win(t_data *data);
void		render(void *arg);
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
void		read_rt(t_data *data);
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
void		final_diffuse(t_color_c *lights, t_light *light, double diff_strength,
				t_vec3 kd);
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
bool		hit_cone(t_obj *co, t_ray ray, t_hit_r *rec);
void		set_co(t_data *data, char *line, int i);
bool		hit_border_cy(t_ray ray, double x_plane, double *t_out);
void		add_point_to_aabb(t_aabb *box, t_vec3 p);
t_aabb		empty_aabb(void);
t_aabb		aabb_transform_matrix(t_aabb local_box, t_mat4 matrix);
void		convert_list_to_arrays(t_data *data);
t_aabb		aabb_cylinder(t_obj *cy);
t_aabb		aabb_hyperboloid(t_obj *hy);
t_aabb		aabb_sphere(t_obj *sp);
t_aabb		aabb_triangle(t_obj *tr);
double		hit_aabb_edge(t_ray ray, t_aabb box);
t_aabb		aabb_square(t_obj *sq);
t_aabb		aabb_cone(t_obj *co);
t_aabb		get_aabb_by_type(t_obj *obj);
t_aabb		compute_bounds(t_obj **objs, int count);
int			cmp_x(const void *a, const void *b);
int			cmp_y(const void *a, const void *b);
int			cmp_z(const void *a, const void *b);
void		build_bvh(t_data *data);
bool		intersect_aabb(t_ray ray, t_aabb box, double t_max);
bool		hit_bvh(t_data *data, int node_idx, t_ray ray, t_hit_r *rec);
void		display_fps(t_data *data);
double		get_time(void);
double		parse_reflectivity(char **line);
double		parse_roughness(char **line);
void		print_progress(int current_line, int total_lines);
t_vec3		get_checker_color(double u, double v, t_vec3 color_a, t_vec3 color_b);
void		get_sphere_uv(t_vec3 normal, double *u, double *v);
void		get_pl_uv(t_vec3 p, t_vec3 normal, double *u, double *v);
void		get_sq_uv(t_vec3 p, t_vec3 center, t_vec3 normal, double side_size, double *u, double *v);
void		get_cycohy_uv(t_vec3 p, t_vec3 center, t_vec3 axis, double height, double *u, double *v);
mlx_color	get_texture_color(t_texture *tex, double u, double v, double scale);
char		*get_texture_path(char **ptr);
t_texture	*load_texture(t_data *data, char *filepath, char *file_o);
void		apply_bump(t_hit_r *rec, t_texture *bump_tex, double strength, double scale);
void		clean(t_data *data);
void		re_init(t_data *data);
void		stop_threads(t_data *data);
void		add_task(t_data *data, t_task func, t_thread_info info);
void		init_thread_p(t_data *data);
t_aabb		aabb_union(t_aabb a, t_aabb b);
int			find_best_split_all_axes(t_data *data, int start, int count);
void		set_ply(t_data *data, char *line, int i);
char		*pars_file_n(char **line);

#endif
