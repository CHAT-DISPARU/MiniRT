/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:27:40 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/26 18:12:01 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	calc_lights(t_color_c *lights, t_hit_r rec, t_data *data, t_light *light)
{
	lights->obj_r = rec.color.r / 255.0;
	lights->obj_g = rec.color.g / 255.0;
	lights->obj_b = rec.color.b / 255.0;
	lights->ambient.x = lights->obj_r * data->a_final.x;// * rec.obj_ptr->ka;
	lights->ambient.y = lights->obj_g * data->a_final.y; //* rec.obj_ptr->ka;
	lights->ambient.z = lights->obj_b * data->a_final.z; //* rec.obj_ptr->ka;
	lights->diffuse.x = light->color.r / 255.0;
	lights->diffuse.y = light->color.g / 255.0;
	lights->diffuse.z = light->color.b / 255.0;
}

void	final_diffuse(t_color_c *lights, t_light *light, double diff_strength)
{
	lights->diffuse.x *= lights->obj_r * light->ratio * diff_strength;
	lights->diffuse.y *= lights->obj_g * light->ratio * diff_strength;
	lights->diffuse.z *= lights->obj_b * light->ratio * diff_strength;
}
