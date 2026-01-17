/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:52:19 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/15 17:27:20 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(int n)
{
	int			i;
	long int	nb;

	i = 1;
	nb = n;
	if (nb < 0)
	{
		i++;
		nb = -nb;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	long int	nb;
	long int	i;
	char		*result;

	nb = n;
	i = int_len(nb);
	result = ft_calloc(i + 1, sizeof(char));
	if (result == NULL)
		return (NULL);
	result[i] = '\0';
	if (nb < 0 && nb != -0)
	{
		nb = -nb;
		result[0] = '-';
	}
	while (nb > 9)
	{
		i--;
		result[i] = (nb % 10) + '0';
		nb = nb / 10;
	}
	i--;
	result[i] = nb + '0';
	return (result);
}
