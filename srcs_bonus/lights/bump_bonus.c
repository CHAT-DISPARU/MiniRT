/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:33:01 by titan             #+#    #+#             */
/*   Updated: 2026/03/10 13:47:56 by gajanvie         ###   ########.fr       */
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
	t_bump_calc	utils;
	t_vec3		bump_vec;
	t_vec3		up_guide;

	up_guide = (t_vec3){0, 1, 0};
	utils.i_center = get_texture_intens(bump_tex, rec->u, rec->v);
	utils.i_u = get_texture_intens(bump_tex,
			rec->u + (1.0 / bump_tex->width), rec->v);
	utils.i_v = get_texture_intens(bump_tex, rec->u,
			rec->v + (1.0 / bump_tex->height));
	utils.d_u = (utils.i_center - utils.i_u) * strength;
	utils.d_v = (utils.i_center - utils.i_v) * strength;
	if (fabs(vec_dot_scal(rec->normal, up_guide)) > 0.99)
		up_guide = (t_vec3){0, 0, 1};
	utils.tangent = vec_normalize(vec_cross(up_guide, rec->normal));
	utils.bitangent = vec_normalize(vec_cross(rec->normal, utils.tangent));
	bump_vec = vec_add(vec_scale(utils.tangent, utils.d_u),
			vec_scale(utils.bitangent, utils.d_v));
	rec->normal = vec_normalize(vec_add(rec->normal, bump_vec));
}
