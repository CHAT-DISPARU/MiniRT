/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 16:33:53 by titan             #+#    #+#             */
/*   Updated: 2026/03/11 10:40:18 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	check_norm_vec(t_data *data, t_vec3 v, int i)
{
	if (v.x < -1.0 || v.x > 1.0
		|| v.y < -1.0 || v.y > 1.0 || v.z < -1.0 || v.z > 1.0)
		clean_exit(data, 1,
			"Error: Normalized vector mut be in range [-1, 1]\n", i);
	if (v.x == 0.0 && v.y == 0.0 && v.z == 0.0)
		clean_exit(data, 1,
			"Error: Orientation vector cannot be zero (0,0,0)\n", i);
}

void	check_missing_info(t_data *data, char *line, int i)
{
	if (!line || *line == '\0' || *line == '\n')
		clean_exit(data, 1,
			"Error: Missing information online\n", i);
}

void	check_ratio(t_data *data, double ratio, int i)
{
	if (ratio < 0.0 || ratio > 1.0)
		clean_exit(data, 1,
			"Error: Ratio must be between 0.0 and 1.0\n", i);
}

void	check_fov(t_data *data, double fov, int i)
{
	if (fov < 0.0 || fov > 180.0)
		clean_exit(data, 1,
			"Error: fov must be between 0 and 180\n", i);
}
