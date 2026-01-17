/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_view.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:22:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/17 18:27:11 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_mat.h"

t_mat4	mat4_view(t_vec3 from, t_vec3 to, t_vec3 up)
{
	t_mat4	view;
	t_vec3	forward;
	t_vec3	left;
	t_vec3	true_up;

	forward = vec_sub(to, from);
	forward = vec_normalize(forward);
	left = vec_cross(forward, vec_normalize(up));
	left = vec_normalize(left);
	true_up = vec_cross(left, forward);
	mat4_initial(&view);
	view.m[0][0] = left.x;
	view.m[0][1] = left.y;
	view.m[0][2] = left.z;
	view.m[1][0] = true_up.x;
	view.m[1][1] = true_up.y;
	view.m[1][2] = true_up.z;
	view.m[2][0] = -forward.x;
	view.m[2][1] = -forward.y;
	view.m[2][2] = -forward.z;
	view.m[0][3] = -vec_dot(left, from);
	view.m[1][3] = -vec_dot(true_up, from);
	view.m[2][3] = vec_dot(forward, from);
	return (view);
}
