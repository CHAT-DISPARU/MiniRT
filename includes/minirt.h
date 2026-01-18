/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:42:01 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/18 13:46:19 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "libft.h"
# include "gajanvielib_math.h"
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

typedef struct s_sphere
{
	t_vec3	center;
	double	radius;
	t_color	color;
}				t_sphere;

typedef struct s_lerp_color
{
	unsigned char	r1;
	unsigned char	g1;
	unsigned char	b1;
	unsigned char	r2;
	unsigned char	g2;
	unsigned char	b2;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}				t_lerp_color;

typedef struct s_data
{
	int				width;
	int				height;
	mlx_context		mlx;
	void			*win;
	void			*img;
	mlx_color		*pixels;
	bool			is_full;
	int				key_table[512];
	int				old_key_table[512];
	t_mat4			camera_to_world;
	t_sphere		test_sphere;
	unsigned int	color_start;
	unsigned int	color_end;
}				t_data;

int		resize_win(t_data *data);
void	render(t_data *data);

#endif
