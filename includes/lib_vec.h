/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_vec.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:24:37 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/19 16:43:12 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_VEC_H
# define LIB_VEC_H

# include <gajanvielib_math.h>
# include <stdio.h>

t_vec3	vec_new(double x, double y, double z);
void	vec_print(char *name, t_vec3 v);
t_vec3	vec_add(t_vec3 v1, t_vec3 v2);
t_vec3	vec_sub(t_vec3 v1, t_vec3 v2);
t_vec3	vec_scale(t_vec3 v, double s);
t_vec3	vec_mult(t_vec3 v1, t_vec3 v2);
double	vec_len(t_vec3 v);
double	vec_len_sq(t_vec3 v);
t_vec3	vec_normalize(t_vec3 v);
double	vec_dot_scal(t_vec3 v1, t_vec3 v2);
t_vec3	vec_cross(t_vec3 v1, t_vec3 v2);
t_vec3	vec_reflect(t_vec3 v, t_vec3 n);
t_vec3	vec_min(t_vec3 v1, t_vec3 v2);
t_vec3	vec_max(t_vec3 v1, t_vec3 v2);

#endif
