/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_something_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:00:06 by titan             #+#    #+#             */
/*   Updated: 2026/02/02 12:23:18 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	new_rec(t_hit_r *rec, t_hit_some *hit, t_obj *objs)
{
	hit->hit = true;
	*rec = hit->tmp_rec;
	rec->color = objs->color;
	hit->closest = hit->tmp_rec.t;
}

bool	hit_someting(t_data *data, t_ray ray, t_hit_r *rec)
{
	t_hit_some		hit;
	t_obj			*objs;
	static t_calc_f	functions[FLAG_MAX];
	static bool		is_init = false;

	if (!is_init)
	{
		init_t_calc_f(functions);
		is_init = true;
	}
	hit.hit = false;
	objs = data->objs;
	hit.closest = INFINITY;
	while (objs)
	{
		if (functions[objs->type](objs, ray, &hit.tmp_rec))
		{
			if (hit.tmp_rec.t < hit.closest)
				new_rec(rec, &hit, objs);
		}
		objs = objs->next;
	}
	return (hit.hit);
}
