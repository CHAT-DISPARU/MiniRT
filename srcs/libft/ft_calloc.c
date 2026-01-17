/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 08:25:07 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/15 17:25:19 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*tab;

	if (nmemb != 0 && size != 0 && ((nmemb * size) / size != nmemb))
		return (NULL);
	if (nmemb == 0 || size == 0)
	{
		tab = malloc(0);
		return (tab);
	}
	tab = (unsigned char *)malloc(nmemb * size);
	if (!tab)
		return (NULL);
	ft_bzero((void *)tab, size * nmemb);
	return (tab);
}
