/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:41:00 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/19 12:05:47 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*s_one;
	unsigned char	*s_two;

	s_one = (unsigned char *)s1;
	s_two = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (s_one[i] != s_two[i] || s_one[i] == '\0')
			return (s_one[i] - s_two[i]);
		i++;
	}
	return (0);
}
