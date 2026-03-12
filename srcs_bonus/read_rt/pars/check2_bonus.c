/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 16:34:08 by titan             #+#    #+#             */
/*   Updated: 2026/03/11 10:42:45 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	check_positive(t_data *data, double val, int i)
{
	if (val <= 0.0)
		clean_exit(data, 1,
			"Error: Dimensions must be positive\n", i);
}

void	check_extra_info(t_data *data, char *line, int i)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t' && *line != '\n')
			clean_exit(data, 1, "Error: Extra information on line\n", i);
		line++;
	}
}

void	check_color_val(t_data *data, double c, int i)
{
	if (c < 0.0 || c > 255.0)
		clean_exit(data, 1,
			"Error: Color values must be between 0 and 255\n", i);
}

double	parse_roughness(char **line)
{
	double	roughness;
	char	*tmp;

	skip_spaces(line);
	tmp = *line;
	if (*tmp == '\0' || *tmp == '\n' || (!ft_isdigit(*tmp) && *tmp != '-'))
		return (0.0);
	roughness = rt_atod(line);
	if (roughness < 0.0)
		return (0.0);
	if (roughness > 1.0)
		return (1.0);
	return (roughness);
}

double	parse_reflectivity(char **line)
{
	double	reflect;
	char	*tmp;

	skip_spaces(line);
	tmp = *line;
	if (*tmp == '\0' || *tmp == '\n' || (!ft_isdigit(*tmp) && *tmp != '-'))
		return (0.0001);
	reflect = rt_atod(line);
	if (reflect < 0.0)
		return (0.0);
	if (reflect > 1.0)
		return (1.0);
	return (reflect);
}
