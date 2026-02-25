/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:27:40 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/02 11:33:16 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

void	calc_lights(t_color_c *lights, t_hit_r rec, t_data *data)
{
	lights->obj_r = rec.color.r / 255.0;
	lights->obj_g = rec.color.g / 255.0;
	lights->obj_b = rec.color.b / 255.0;
	lights->ambient.x = lights->obj_r * data->a_final.x;
	lights->ambient.y = lights->obj_g * data->a_final.y;
	lights->ambient.z = lights->obj_b * data->a_final.z;
	lights->diffuse.x = data->light.color.r / 255.0;
	lights->diffuse.y = data->light.color.g / 255.0;
	lights->diffuse.z = data->light.color.b / 255.0;
}

void	final_diffuse(t_color_c *lights, t_data *data, double diff_strength)
{
	lights->diffuse.x *= lights->obj_r * data->light.ratio * diff_strength;
	lights->diffuse.y *= lights->obj_g * data->light.ratio * diff_strength;
	lights->diffuse.z *= lights->obj_b * data->light.ratio * diff_strength;
}
