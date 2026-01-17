/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 09:32:43 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/19 12:35:23 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	i;
	size_t	start;
	size_t	end;

	i = 0;
	start = 0;
	end = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && ft_strchr(set, s1[start]))
		start ++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	result = (char *)malloc((end - start + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (start < end)
		result[i++] = s1[start++];
	result[i] = '\0';
	return (result);
}
