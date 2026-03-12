/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_part2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:28:54 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:29:28 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	atoi_move(char **s)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	if (**s == '-')
	{
		sign = -1;
		(*s)++;
	}
	while (**s >= '0' && **s <= '9')
	{
		res = res * 10 + (**s - '0');
		(*s)++;
	}
	return (res * sign);
}

void	fill_face_v(char **s, int idx[3][3], int i)
{
	while (**s && !ft_isdigit(**s))
		(*s)++;
	idx[0][i] = atoi_move(s);
	idx[1][i] = 0;
	idx[2][i] = 0;
	if (**s == '/')
	{
		(*s)++;
		if (**s != '/')
			idx[1][i] = atoi_move(s);
		if (**s == '/')
		{
			(*s)++;
			idx[2][i] = atoi_move(s);
		}
	}
	while (**s && !is_space(**s) && **s != '\n')
		(*s)++;
}

t_triangle	parse_face_fast(char **s, t_vars_obj *v)
{
	t_triangle	tri;
	int			idx[3][3];
	int			i;

	i = -1;
	while (++i < 3)
		fill_face_v(s, idx, i);
	tri.p1 = mat4_mult_vec3(&v->t.final, v->v[idx[0][0] - 1], 1.0);
	tri.p2 = mat4_mult_vec3(&v->t.final, v->v[idx[0][1] - 1], 1.0);
	tri.p3 = mat4_mult_vec3(&v->t.final, v->v[idx[0][2] - 1], 1.0);
	if (idx[1][0] > 0 && v->vt)
	{
		tri.uv1 = (t_vec2){v->vt[idx[1][0] - 1].x, v->vt[idx[1][0] - 1].y};
		tri.uv2 = (t_vec2){v->vt[idx[1][1] - 1].x, v->vt[idx[1][1] - 1].y};
		tri.uv3 = (t_vec2){v->vt[idx[1][2] - 1].x, v->vt[idx[1][2] - 1].y};
	}
	if (idx[2][0] > 0 && v->vn)
		tri.normal = vec_normalize(mat4_mult_vec3(&v->t.final,
					v->vn[idx[2][0] - 1], 0.0));
	else
		tri.normal = vec_normalize(vec_cross(vec_sub(tri.p2, tri.p1),
					vec_sub(tri.p3, tri.p1)));
	return (tri);
}

char	*pars_file_n(char **line)
{
	int		len;
	char	*result;

	while (**line && is_space(**line))
		(*line)++;
	len = 0;
	while ((*line)[len] && !is_space((*line)[len]) && (*line)[len] != '\n')
		len++;
	if (len == 0)
		return (NULL);
	result = ft_calloc(len + 1, sizeof(char));
	ft_strlcpy(result, *line, len + 1);
	*line += len;
	return (result);
}

void	set_base_mat(t_mtl_info *mat)
{
	mat->idx = NULL;
	mat->ka = (t_vec3){0.2, 0.2, 0.2};
	mat->ks = (t_vec3){1, 1, 1};
	mat->kd = (t_vec3){0.8, 0.8, 0.8};
	mat->ns = 32;
	mat->ni = -1;
	mat->tex = NULL;
	mat->bump = NULL;
	mat->has_col = false;
	mat->color = (mlx_color)(uint32_t){0xFFFFFFFF};
	mat->reflectivity = -1.0;
	mat->rought = -1.0;
	mat->opacity = -1.0;
}
