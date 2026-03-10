/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:27:40 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/10 14:00:40 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	calc_lights(t_color_c *lights, t_hit_r rec, t_data *data)
{
	lights->obj_r = rec.color.r / 255.0;
	lights->obj_g = rec.color.g / 255.0;
	lights->obj_b = rec.color.b / 255.0;
	lights->ambient.x = lights->obj_r * data->a_final.x * rec.obj_ptr->ka.x;
	lights->ambient.y = lights->obj_g * data->a_final.y * rec.obj_ptr->ka.y;
	lights->ambient.z = lights->obj_b * data->a_final.z * rec.obj_ptr->ka.z;
}

void	final_diffuse(t_color_c *lights,
	t_light *light, double diff_strength, t_vec3 kd)
{
	lights->diffuse.x *= lights->obj_r * light->ratio * diff_strength * kd.x;
	lights->diffuse.y *= lights->obj_g * light->ratio * diff_strength * kd.y;
	lights->diffuse.z *= lights->obj_b * light->ratio * diff_strength * kd.z;
}
