/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:11:53 by gajanvie          #+#    #+#             */
/*   Updated: 2025/12/15 11:40:37 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (f)(unsigned int, char *), size_t len)
{
	size_t	i;

	i = 0;
	if (!f || !s)
		return ;
	while (i < len)
	{
		(*f)(i, s + i);
		i++;
	}
}
