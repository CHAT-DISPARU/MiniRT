/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_norm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:34:22 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/17 18:27:11 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib_vec.h>

t_vec3	vec_normalize(t_vec3 v)
{
	double	len;
	double	inv_len;

	len = vec_len(v);
	if (len == 0.0)
		return ((t_vec3){0, 0, 0});
	inv_len = 1.0 / len;
	return ((t_vec3){v.x * inv_len, v.y * inv_len, v.z * inv_len});
}
