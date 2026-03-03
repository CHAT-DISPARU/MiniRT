/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:33:01 by titan             #+#    #+#             */
/*   Updated: 2026/03/01 14:31:03 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

double	get_texture_intens(t_texture *bump_tex, double u, double v)
{
	mlx_color	color;

	color = get_texture_color(bump_tex, u, v);
	return ((double)(color.r + color.g + color.b) / 765);
}

/*
	strength : La frce du relief 
	bump_tex : La texture de bump
	Nouvelle Normale = Ancienne + (PenteU * Tangente) + (PenteV * Bitangente)
*/
void	apply_bump(t_hit_r *rec, t_texture *bump_tex, double strength)
{
	double	i_center;
	double	i_u;
	double	i_v;
	t_vec3	tangent;
	t_vec3	bitangent;
	t_vec3	bump_vec;
	t_vec3	up_guide;

	up_guide = (t_vec3){0, 1, 0};
	i_center = get_texture_intens(bump_tex, rec->u, rec->v);
	i_u = get_texture_intens(bump_tex,
		rec->u + (1.0 / bump_tex->width), rec->v);
	i_v = get_texture_intens(bump_tex, rec->u,
		rec->v + (1.0 / bump_tex->height));
	double d_u = (i_center - i_u) * strength;
	double d_v = (i_center - i_v) * strength;
	if (fabs(vec_dot_scal(rec->normal, up_guide)) > 0.99)
		up_guide = (t_vec3){0, 0, 1};
	tangent = vec_normalize(vec_cross(up_guide, rec->normal));
	bitangent = vec_normalize(vec_cross(rec->normal, tangent));
	bump_vec = vec_add(vec_scale(tangent, d_u), vec_scale(bitangent, d_v));
	rec->normal = vec_normalize(vec_add(rec->normal, bump_vec));
}
