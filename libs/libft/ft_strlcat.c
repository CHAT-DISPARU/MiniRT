/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:10:33 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/19 12:08:28 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;

	s_len = ft_strlen(src);
	d_len = ft_strlen(dest);
	if (d_len >= size)
		d_len = size;
	if (d_len == size)
		return (size + s_len);
	if (s_len < size - d_len)
		ft_memcpy(dest + d_len, src, s_len + 1);
	else
	{
		ft_memcpy(dest + d_len, src, size - d_len - 1);
		dest[size - 1] = '\0';
	}
	return (d_len + s_len);
}
