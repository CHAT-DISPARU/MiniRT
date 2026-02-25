/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gajanvielib_math.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:50:56 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/13 15:48:01 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAJANVIELIB_MATH_H
# define GAJANVIELIB_MATH_H

# include <math.h>

typedef struct s_2index
{
	int	i;
	int	j;
}				t_2index;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef struct s_vec2
{
	double	x;
	double	y;
}				t_vec2;

typedef struct s_mat4
{
	double	m[4][4];
}				t_mat4;

typedef t_vec3	t_point;
typedef t_vec3	t_color;

#endif
