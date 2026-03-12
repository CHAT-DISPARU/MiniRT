/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_part4_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:34:27 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:34:38 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	set_obj_type2(char **c, t_vars_obj *v, t_data *data)
{
	if (*(*c + 1) == 't')
	{
		*c += 3;
		if (v->idx[2] >= v->c_vt)
		{
			v->c_vt *= 2;
			v->vt = realloc(v->vt, sizeof(t_vec3) * v->c_vt);
			if (!v->vt)
				error_map_mtl(*v, data, 1);
		}
		v->vt[v->idx[2]++] = parse_vec_fast(c, 2);
	}
}

void	set_obj_type(char **c, t_vars_obj *v, t_data *data)
{
	if (*(*c + 1) == ' ')
	{
		*c += 2;
		if (v->idx[0] >= v->c_v)
		{
			v->c_v *= 2;
			v->v = realloc(v->v, sizeof(t_vec3) * v->c_v);
			if (!v->vt)
				error_map_mtl(*v, data, 1);
		}
		v->v[v->idx[0]++] = parse_vec_fast(c, 1);
	}
	else if (*(*c + 1) == 'n')
	{
		*c += 3;
		if (v->idx[1] >= v->c_vn)
		{
			v->c_vn *= 2;
			v->vn = realloc(v->vn, sizeof(t_vec3) * v->c_vn);
			if (!v->vt)
				error_map_mtl(*v, data, 1);
		}
		v->vn[v->idx[1]++] = parse_vec_fast(c, 1);
	}
	set_obj_type2(c, v, data);
}
