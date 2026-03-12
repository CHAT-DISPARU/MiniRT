/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_rt_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:04:41 by titan             #+#    #+#             */
/*   Updated: 2026/03/11 10:40:18 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

char	*jump_to_next_line(char **cursor)
{
	char		*line_start;

	line_start = *cursor;
	while (**cursor && **cursor != '\n')
		*cursor += 1;
	if (**cursor == '\n')
	{
		**cursor = '\0';
		*cursor += 1;
	}
	return (line_start);
}

bool	verify_filename(char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 3)
		return (false);
	if (!ft_strncmp(filename + len - 3, ".rt", 3))
		return (true);
	return (false);
}

void	set_rt_file_read(t_data *data, struct stat *file_stat)
{
	if (verify_filename(data->filename) == false)
		clean_exit(data, 1, "Error: File name\n", 0);
	data->scene_fd = open(data->filename, O_RDONLY);
	if (data->scene_fd < 0)
		clean_exit(data, 1, "Error: Open failed\n", 0);
	if (fstat(data->scene_fd, file_stat) < 0)
		clean_exit(data, 1, "Error: Fstat failed\n", 0);
	if (S_ISDIR(file_stat->st_mode))
		clean_exit(data, 1, "Error: Is a directory\n", 0);
	data->scene_line = malloc(sizeof(char) * (file_stat->st_size + 1));
	if (!data->scene_line)
		clean_exit(data, 1, "Error: Malloc failed\n", 0);
	if (read(data->scene_fd, data->scene_line, file_stat->st_size) == -1)
		clean_exit(data, 1, "Error: Read failed\n", 0);
	data->scene_line[file_stat->st_size] = '\0';
	close(data->scene_fd);
	data->scene_fd = -1;
}

void	read_rt(t_data *data)
{
	struct stat	file_stat;
	char		*cursor;
	int			i;
	char		*ptr;

	set_rt_file_read(data, &file_stat);
	i = 1;
	ptr = NULL;
	cursor = data->scene_line;
	while (*cursor)
	{
		ptr = jump_to_next_line(&cursor);
		while (*ptr && is_space(*ptr))
			ptr++;
		if (*ptr)
			call_setters(data, ptr, i);
		if (i % 500 == 0)
			printf("\rLoading line: %d...", i);
		i++;
	}
	free(data->scene_line);
	data->scene_line = NULL;
	printf("\nParsing finished: %d lines loaded.\n", i);
}
