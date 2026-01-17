/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_len.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:34:15 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/17 18:27:11 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib_vec.h>

/*
	len au carre
*/

double	vec_len_sq(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

double	vec_full_len(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}
