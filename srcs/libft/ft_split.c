/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:30:37 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/18 16:38:53 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	if (c == ' ')
		return (1);
	if (c >= 7 && c <= 13)
		return (1);
	return (0);
}

static size_t	nbr_word(const char *str)
{
	size_t	i;
	size_t	result;

	i = 0;
	result = 0;
	while (str != NULL && str[i])
	{
		if (!is_space(str[i]))
		{
			result ++;
			while (str[i] && !is_space(str[i]))
				i ++;
		}
		else
			i ++;
	}
	return (result);
}

void	*free_all(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (NULL);
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static char	*ft_split_word(const char *str)
{
	char	*tab;
	size_t	i;

	i = 0;
	while (str[i] && !is_space(str[i]))
		i ++;
	tab = ft_calloc(i + 1, sizeof(char));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (str[i] && !is_space(str[i]))
	{
		tab[i] = str[i];
		i ++;
	}
	return (tab);
}

char	**ft_split(char const *s)
{
	char	**tab;
	size_t	i;
	size_t	k;

	if (!s)
		return (NULL);
	i = 0;
	k = 0;
	tab = ft_calloc(nbr_word(s) + 1, sizeof(char *));
	while (tab && s[i])
	{
		if (!is_space(s[i]))
		{
			tab[k] = ft_split_word(&s[i]);
			if (!tab[k])
			{
				free_all(tab);
				return (NULL);
			}
			i += ft_strlen(tab[k++]);
			continue ;
		}
		i++;
	}
	return (tab);
}
