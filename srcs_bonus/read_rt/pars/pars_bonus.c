/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:17:03 by titan             #+#    #+#             */
/*   Updated: 2026/03/03 13:07:59 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_mat4	mat4_align_vectors(t_vec3 start, t_vec3 dest)
{
	t_vec3	axis;
	double	cos_theta;
	t_mat4	rot;
	t_vec3	up;

	start = vec_normalize(start);
	dest = vec_normalize(dest);
	cos_theta = vec_dot_scal(start, dest);
	if (cos_theta > 0.9999)
	{
		mat4_initial(&rot);
		return (rot);
	}
	if (cos_theta < -0.9999)
	{
		up = (t_vec3){0, 1, 0};
		if (fabs(start.y) > 0.9)
			up = (t_vec3){0, 0, 1};
		axis = vec_cross(start, up);
		mat4_rotate_axis(&rot, axis, PI);
		return (rot);
	}
	axis = vec_cross(start, dest);
	mat4_rotate_axis(&rot, axis, acos(cos_theta));
	return (rot);
}

t_vec3	parse_vec3(char **line, t_data *data, int i)
{
	t_vec3	v;

	v.x = rt_atod(line);
	skip_coma(line, data, i);
	v.y = rt_atod(line);
	skip_coma(line, data, i);
	v.z = rt_atod(line);
	return (v);
}

mlx_color	parse_color(char **line, t_data *data, int i)
{
	mlx_color	color;
	double		r;
	double		g;
	double		b;

	r = rt_atod(line);
	color.r = r;
	check_color_val(data, r, i);
	skip_coma(line, data, i);
	g = rt_atod(line);
	color.g = g;
	check_color_val(data, g, i);
	skip_coma(line, data, i);
	b = rt_atod(line);
	color.b = b;
	check_color_val(data, b, i);
	return (color);
}

char	*get_texture_path(char **ptr)
{
	int		len;
	int		j;
	char	*path;

	while (**ptr && is_space(**ptr))
		(*ptr)++;
	if (!**ptr || **ptr == '\n' || **ptr == '\r')
		return (NULL);
	len = 0;
	while ((*ptr)[len] && !is_space((*ptr)[len]) && ((*ptr)[len] != '\n' || (*ptr)[len] != '\r'))
		len++;
	path = malloc(sizeof(char) * (len + 1));
	if (!path)
		return (NULL);
	j = 0;
	while (j < len)
	{
		path[j] = **ptr;
		(*ptr)++;
		j++;
	}
	path[j] = '\0';
	return (path);
}
