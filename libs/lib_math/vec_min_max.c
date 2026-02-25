/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_min_max.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 18:08:34 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/17 18:19:42 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_vec.h"

t_vec3	vec_min(t_vec3 v1, t_vec3 v2)
{
	t_vec3	res;

	if (v1.x < v2.x)
		res.x = v1.x;
	else
		res.x = v2.x;
	if (v1.y < v2.y)
		res.y = v1.y;
	else
		res.y = v2.y;
	if (v1.z < v2.z)
		res.z = v1.z;
	else
		res.z = v2.z;
	return (res);
}

t_vec3	vec_max(t_vec3 v1, t_vec3 v2)
{
	t_vec3	res;

	if (v1.x > v2.x)
		res.x = v1.x;
	else
		res.x = v2.x;
	if (v1.y > v2.y)
		res.y = v1.y;
	else
		res.y = v2.y;
	if (v1.z > v2.z)
		res.z = v1.z;
	else
		res.z = v2.z;
	return (res);
}
