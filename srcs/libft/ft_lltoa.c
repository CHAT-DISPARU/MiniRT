/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntome <ntome@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 20:31:45 by ntome             #+#    #+#             */
/*   Updated: 2025/12/12 20:36:19 by ntome            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	long_long_len(long long int n)
{
	int				i;
	long long int	nb;

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

char	*ft_lltoa(long long int n)
{
	long long int	nb;
	long long int	i;
	char			*result;

	nb = n;
	i = long_long_len(nb);
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
