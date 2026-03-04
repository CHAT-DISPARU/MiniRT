/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_cost.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:02:25 by titan             #+#    #+#             */
/*   Updated: 2026/03/04 14:47:36 by gajanvie         ###   ########.fr       */
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

static int	sah_axis(t_obj **objs, int count, int axis, double *out_cost)
{
	t_aabb	*prefix;
	t_aabb	*suffix;
	double	min_cost;
	double	cost;
	int		best_mid;
	int		i;

	prefix = malloc(sizeof(t_aabb) * count);
	suffix = malloc(sizeof(t_aabb) * count);
	if (!prefix || !suffix)
	{
		free(prefix);
		free(suffix);
		*out_cost = DBL_MAX;
		return (count / 2);
	}
	if (axis == 0)
		qsort(objs, count, sizeof(t_obj *), cmp_x);
	else if (axis == 1)
		qsort(objs, count, sizeof(t_obj *), cmp_y);
	else
		qsort(objs, count, sizeof(t_obj *), cmp_z);
	prefix[0] = get_aabb_by_type(objs[0]);
	i = 1;
	while (i < count)
	{
		prefix[i] = aabb_union(prefix[i - 1], get_aabb_by_type(objs[i]));
		i++;
	}
	suffix[count - 1] = get_aabb_by_type(objs[count - 1]);
	i = count - 2;
	while (i >= 0)
	{
		suffix[i] = aabb_union(suffix[i + 1], get_aabb_by_type(objs[i]));
		i--;
	}
	min_cost = DBL_MAX;
	best_mid = count / 2;
	i = 1;
	while (i < count)
	{
		cost = get_aabb_surface_area(prefix[i - 1]) * (double)i
			 + get_aabb_surface_area(suffix[i])     * (double)(count - i);
		if (cost < min_cost)
		{
			min_cost = cost;
			best_mid = i;
		}
		i++;
	}
	free(prefix);
	free(suffix);
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
