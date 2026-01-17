/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:30:37 by gajanvie          #+#    #+#             */
/*   Updated: 2025/12/11 15:22:57 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	nbr_word(const char *str, char c)
{
	size_t	i;
	size_t	result;

	i = 0;
	result = 0;
	while (str != NULL && str[i])
	{
		if (str[i] != c)
		{
			result ++;
			while (str[i] && str[i] != c)
				i ++;
		}
		else
			i ++;
	}
	return (result);
}

void	free_all(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static char	*ft_split_word(const char *str, char sep)
{
	char	*tab;
	size_t	i;

	i = 0;
	while (str[i] && str[i] != sep)
		i ++;
	tab = ft_calloc(i + 1, sizeof(char));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != sep)
	{
		tab[i] = str[i];
		i ++;
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	size_t	i;
	size_t	k;

	if (!s)
		return (NULL);
	i = 0;
	k = 0;
	tab = ft_calloc(nbr_word(s, c) + 1, sizeof(char *));
	while (tab && s[i])
	{
		if (s[i] != c)
		{
			tab[k] = ft_split_word(&s[i], c);
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
