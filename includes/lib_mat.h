/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_mat.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:53:05 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/17 18:07:15 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_MAT_H
# define LIB_MAT_H

# include <gajanvielib_math.h>
# include <lib_vec.h>
# include <stdio.h>

void	mat4_initial(t_mat4 *mat);
t_mat4	mat4_mult(t_mat4 *a, t_mat4 *b);
void	mat4_rotate_z(t_mat4 *m, float angle);
void	mat4_rotate_x(t_mat4 *m, float angle);
void	mat4_rotate_y(t_mat4 *m, float angle);
void	print_mat(t_mat4 m);
void	mat4_translation(t_mat4 *m, t_vec3 t);
void	mat4_scal(t_mat4 *m, t_vec3 s);
t_vec3	mat4_mult_vec3(t_mat4 *m, t_vec3 v, double w);
t_mat4	mat4_trans(t_mat4 *m);
double	mat4_deter(t_mat4 *m);
t_mat4	mat4_inverse(t_mat4 *m);
t_mat4	mat4_view(t_vec3 from, t_vec3 to, t_vec3 up);

#endif
