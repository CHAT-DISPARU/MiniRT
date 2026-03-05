/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_cost.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:02:25 by titan             #+#    #+#             */
/*   Updated: 2026/03/05 11:31:54 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

double	get_aabb_surface_area(t_aabb box)
{
	t_vec3	size;

	size = vec_sub(box.max, box.min);
	return (2.0 * ((size.x * size.y) + (size.y * size.z) + (size.z * size.x)));
}

/*
	Surface Area Heuristic (SAH)
	Cout=(Aire Gauche ​× NGauche​) + (Aire Droite​ × NDroite​)
*/

void	sort_and_set(int count, int axis, t_sah *sah_utils, t_obj **objs)
{
	if (axis == 0)
		qsort(objs, count, sizeof(t_obj *), cmp_x);
	else if (axis == 1)
		qsort(objs, count, sizeof(t_obj *), cmp_y);
	else
		qsort(objs, count, sizeof(t_obj *), cmp_z);
	sah_utils->prefix[0] = get_aabb_by_type(objs[0]);
	sah_utils->i = 1;
	while (sah_utils->i < count)
	{
		sah_utils->prefix[sah_utils->i] = aabb_union(sah_utils->prefix[sah_utils->i - 1],
				get_aabb_by_type(objs[sah_utils->i]));
		sah_utils->i++;
	}
	sah_utils->suffix[count - 1] = get_aabb_by_type(objs[count - 1]);
	sah_utils->i = count - 2;
	while (sah_utils->i >= 0)
	{
		sah_utils->suffix[sah_utils->i] = aabb_union(sah_utils->suffix[sah_utils->i + 1],
				get_aabb_by_type(objs[sah_utils->i]));
		sah_utils->i--;
	}
}

bool	malloc_s_e(double *out_cost, int count, t_sah *sah_utils)
{
	sah_utils->prefix = malloc(sizeof(t_aabb) * count);
	sah_utils->suffix = malloc(sizeof(t_aabb) * count);
	if (!sah_utils->prefix || !sah_utils->suffix)
	{
		free(sah_utils->prefix);
		free(sah_utils->suffix);
		*out_cost = DBL_MAX;
		return (false);
	}
	return (true);
}

static int	sah_axis(t_obj **objs, int count, int axis, double *out_cost)
{
	t_sah	sah_utils;
	double	min_cost;
	double	cost;
	int		best_mid;

	if (malloc_s_e(out_cost, count, &sah_utils) == false)
		return (count / 2);
	sort_and_set(count, axis, &sah_utils, objs);
	min_cost = DBL_MAX;
	best_mid = count / 2;
	sah_utils.i = 1;
	while (sah_utils.i < count)
	{
		cost = get_aabb_surface_area(sah_utils.prefix[sah_utils.i - 1]) * (double)sah_utils.i
			 + get_aabb_surface_area(sah_utils.suffix[sah_utils.i])     * (double)(count - sah_utils.i);
		if (cost < min_cost)
		{
			min_cost = cost;
			best_mid = sah_utils.i;
		}
		sah_utils.i++;
	}
	free(sah_utils.prefix);
	free(sah_utils.suffix);
	*out_cost = min_cost;
	return (best_mid);
}

void	setmidaxis(int *best_axis, int count, t_obj ***tmp, int *mid)
{
	double	cost[3];

	mid[0] = sah_axis(tmp[0], count, 0, &cost[0]);
	mid[1] = sah_axis(tmp[1], count, 1, &cost[1]);
	mid[2] = sah_axis(tmp[2], count, 2, &cost[2]);
	*best_axis = 0;
	if (cost[1] < cost[*best_axis])
		*best_axis = 1;
	if (cost[2] < cost[*best_axis])
		*best_axis = 2;
}

int	find_best_split_all_axes(t_data *data, int start, int count)
{
	t_obj	**tmp[3];
	int		mid[3];
	int		best_axis;
	int		i;

	i = 0;
	while (i < 3)
	{
		tmp[i] = malloc(sizeof(t_obj *) * count);
		if (!tmp[i])
		{
			while (--i >= 0)
				free(tmp[i]);
			return (count / 2);
		}
		ft_memcpy(tmp[i], &data->sorted_objs[start], sizeof(t_obj *) * count);
		i++;
	}
	setmidaxis(&best_axis, count, tmp, mid);
	ft_memcpy(&data->sorted_objs[start], tmp[best_axis], sizeof(t_obj *) * count);
	free(tmp[0]);
	free(tmp[1]);
	free(tmp[2]);
	return (mid[best_axis]);
}
