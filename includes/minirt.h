/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:42:01 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/30 13:13:18 by titan            ###   ########.fr       */
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

typedef struct s_camera
{
	t_vec3	origin;
	t_vec3	dir;
	t_vec3	up_guide;
	double	fov;
}				t_camera;

typedef struct s_hit_r
{
	t_vec3	p;
	t_vec3	normal;
	double	t;
}				t_hit_r;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	dir;
}				t_ray;

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
	int				type;
	double			ratio;
	struct s_light	*next;
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
	int				width;
	int				height;
	mlx_context		mlx;
	void			*win;
	void			*img;
	mlx_color		*pixels;
	bool			is_full;
	int				key_table[512];
	int				old_key_table[512];
	t_camera		cam;
	t_obj			*objs;
	t_light			*lights;
	t_alight		alight;
	double			speed;
	double			rot_speed;
	int				s_per_pixs;
	t_view_p		view_port;
}				t_data;

typedef bool	(*t_calc_f)(t_obj *obj, t_ray ray, t_hit_r *rec);
int			resize_win(t_data *data);
void		render(t_data *data);
mlx_color	parse_color(char **line);
t_vec3		parse_vec3(char **line);
double		rt_atod(char **line);
void		skip_coma(char **line);
void		skip_spaces(char **line);
t_mat4		mat4_align_vectors(t_vec3 start, t_vec3 dest);

#endif
