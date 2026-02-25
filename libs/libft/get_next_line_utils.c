/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:59:23 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/14 10:43:36 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>

static void	fill_str(char const *s1, char const *s2, char **result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		(*result)[j] = s1[i];
		j++;
		i++;
	}
	i = 0;
	while (s2[i])
	{
		(*result)[j] = s2[i];
		i++;
		j++;
	}
	(*result)[j] = '\0';
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	result = (char *)malloc(sizeof(char) * (i + j + 1));
	if (!result)
		return (NULL);
	fill_str(s1, s2, &result);
	free(s1);
	return (result);
}
