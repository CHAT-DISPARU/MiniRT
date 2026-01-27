/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:10:15 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/25 18:05:35 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	get_index(char *base, char c)
{
	int	i;

	i = 0;
	while (base[i])
	{
		if (base[i] == c)
			return (i);
		i ++;
	}
	return (-1);
}

int	check_base(char *base)
{
	int	i;
	int	j;

	i = 0;
	while (base[i])
	{
		j = i + 1;
		while (base[j])
		{
			if (base[i] == base[j]
				|| base[i] == '-' || base[i] == '+'
				|| base[j] == '-' || base[j] == '+'
				|| (base[i] < ' ' || base[i] > 126)
				|| (base[j] < ' ' || base[j] > 126))
				return (1);
			j ++;
		}
		i ++;
	}
	if (ft_strlen(base) >= 2)
		return (0);
	return (1);
}

unsigned int	ft_atol_base(char *str, char *base)
{
	int			i;
	long long	result;

	i = 0;
	result = 0;
	if (check_base(base) == 0)
	{
		while (str[i] == ' ' || (str[i] <= 13 && str[i] >= 9))
			i ++;
		while (str[i] == '+' || str[i] == '-')
			i ++;
		while (get_index(base, str[i]) != -1)
		{
			result = (result * ft_strlen(base)) + get_index(base, str[i]);
			i ++;
		}
		return ((unsigned int)result);
	}
	return (0);
}
