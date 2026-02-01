/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 08:37:49 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/19 12:13:15 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*cpy_src;
	int		i;

	i = 0;
	while (src[i])
		i ++;
	cpy_src = (char *)malloc((i + 1) * sizeof(char));
	i = 0;
	if (cpy_src == NULL)
		return (NULL);
	while (src[i])
	{
		cpy_src[i] = src[i];
		i ++;
	}
	cpy_src[i] = '\0';
	return (cpy_src);
}
