/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 19:58:01 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/26 09:28:27 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>

char	*fill_result(char **stash, int len, char *result)
{
	int		i;
	char	*new_stash;

	i = 0;
	while (i < len)
	{
		result[i] = (*stash)[i];
		i++;
	}
	result[i] = '\0';
	if ((*stash)[len] == '\0')
	{
		free(*stash);
		*stash = NULL;
		return (result);
	}
	new_stash = ft_strdup(*stash + len);
	free (*stash);
	*stash = new_stash;
	return (result);
}

char	*ft_get_line(char	**stash)
{
	char	*result;
	int		i;
	int		len;

	i = 0;
	if (!stash || !*stash || **stash == '\0')
		return (NULL);
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	if ((*stash)[i] == '\n')
		len = i + 1;
	else
		len = i;
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result = fill_result(stash, len, result);
	return (result);
}

void	free_stash(char **stash)
{
	free(*stash);
	*stash = NULL;
}

int	read_file(char **stash, int bit_r, char *buffer, int fd)
{
	while (bit_r > 0)
	{
		buffer[bit_r] = '\0';
		*stash = ft_strjoin_gnl(*stash, buffer);
		if (ft_strchr(buffer, '\n'))
			return (bit_r);
		bit_r = read(fd, buffer, BUFFER_SIZE);
	}
	return (bit_r);
}

char	*get_next_line(int fd, int i)
{
	char		*buffer;
	char		*newline;
	static char	*stash[1024];
	int			bit_r;

	if (i == 1)
	{
		free_stash(&stash[fd]);
		return (NULL);
	}
	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bit_r = read(fd, buffer, BUFFER_SIZE);
	bit_r = read_file(&stash[fd], bit_r, buffer, fd);
	free(buffer);
	if (bit_r < 0 || !stash[fd] || stash[fd][0] == '\0')
	{
		free_stash(&stash[fd]);
		return (NULL);
	}
	newline = ft_get_line(&stash[fd]);
	return (newline);
}
