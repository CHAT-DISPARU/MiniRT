/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_reflect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:33:58 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/19 16:44:51 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib_vec.h>

t_vec3	vec_reflect(t_vec3 v, t_vec3 n)
{
	double	dot;
	t_vec3	tmp;

	dot = vec_dot_scal(v, n);
	tmp = vec_scale(n, 2.0 * dot);
	return (vec_sub(v, tmp));
}
