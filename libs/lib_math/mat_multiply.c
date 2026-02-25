/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_multiply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:56:16 by gajanvie          #+#    #+#             */
/*   Updated: 2026/01/17 18:21:34 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib_mat.h>

t_mat4	mat4_mult(t_mat4 *a, t_mat4 *b)
{
	t_mat4	tmp;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			tmp.m[i][j] = 0.0f;
			k = 0;
			while (k < 4)
			{
				tmp.m[i][j] += a->m[i][k] * b->m[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (tmp);
}

/*
	w = 1 -> point
	w = 0 -> vecteur
*/

t_vec3	mat4_mult_vec3(t_mat4 *m, t_vec3 v, double w)
{
	t_vec3	res;

	res.x = v.x * m->m[0][0]
		+ v.y * m->m[0][1] + v.z * m->m[0][2] + w * m->m[0][3];
	res.y = v.x * m->m[1][0]
		+ v.y * m->m[1][1] + v.z * m->m[1][2] + w * m->m[1][3];
	res.z = v.x * m->m[2][0]
		+ v.y * m->m[2][1] + v.z * m->m[2][2] + w * m->m[2][3];
	return (res);
}
