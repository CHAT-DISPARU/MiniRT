/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 08:40:45 by gajanvie          #+#    #+#             */
/*   Updated: 2025/10/15 17:19:37 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;
	size_t	mem_space;

	mem_space = 1;
	if (!s)
		return (NULL);
	while (mem_space + start <= ft_strlen(s) && mem_space <= len)
		mem_space++;
	result = (char *)malloc((mem_space) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len && start + i < ft_strlen(s))
	{
		result[i] = s[i + start];
		i++;
	}
	result[i] = '\0';
	return (result);
}
