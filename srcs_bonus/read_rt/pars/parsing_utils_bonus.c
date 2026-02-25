/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:15:00 by titan             #+#    #+#             */
/*   Updated: 2026/02/02 12:23:18 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	is_space(char c)
{
	return (c == ' '
		|| c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

void	skip_spaces(char **line)
{
	while (**line && is_space(**line))
		(*line)++;
}

void	skip_coma(char **line, t_data *data, int i)
{
	if (**line == ',')
		(*line)++;
	else
		clean_exit(data, 1, "Error: Missing comma in color/vector\n", i);
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
