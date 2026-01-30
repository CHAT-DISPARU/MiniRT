/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:15:00 by titan             #+#    #+#             */
/*   Updated: 2026/01/30 13:30:57 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt.h>

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

void	skip_spaces(char **line)
{
	while (**line && is_space(**line))
		(*line)++;
}

void	skip_coma(char **line)
{
	while (**line && is_space(**line))
		(*line)++;
	if (**line == ',')
		(*line)++;
	while (**line && is_space(**line))
		(*line)++;
}

double	rt_atod(char **line)
{
	double	res;
	double	sign;
	double	div;

	res = 0.0;
	sign = 1.0;
	div = 0.1;
	skip_spaces(line);
	if (**line == '-' && (*line)++)
		sign = -1.0;
	while (ft_isdigit(**line))
		res = res * 10.0 + (*(*line)++) - '0';
	if (**line == '.')
	{
		(*line)++;
		while (ft_isdigit(**line))
		{
			res = res + ((*(*line)++) - '0') * div;
			div *= 0.1;
		}
	}
	return (res * sign);
}
