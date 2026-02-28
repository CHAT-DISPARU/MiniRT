/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_rt_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:04:41 by titan             #+#    #+#             */
/*   Updated: 2026/02/28 12:15:23 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>
#include <sys/stat.h>

void	call_setters(t_data *data, char *ptr, int i)
{
	if (*ptr == 'A' && is_space(ptr[1]))
		set_a(data, ptr, i);
	else if (*ptr == 'O' && is_space(ptr[1]))
		set_o(data, ptr, i);
	else if (*ptr == 'C' && is_space(ptr[1]))
		set_c(data, ptr, i);
	else if (*ptr == 'L' && is_space(ptr[1]))
		set_l(data, ptr, i);
	else if (!ft_strncmp("sp", ptr, 2) && is_space(ptr[2]))
		set_sp(data, ptr + 2, i);
	else if (!ft_strncmp("pl", ptr, 2) && is_space(ptr[2]))
		set_pl(data, ptr + 2, i);
	else if (!ft_strncmp("cy", ptr, 2) && is_space(ptr[2]))
		set_cy(data, ptr + 2, i);
	else if (!ft_strncmp("sq", ptr, 2) && is_space(ptr[2]))
		set_sq(data, ptr + 2, i);
	else if (!ft_strncmp("hy", ptr, 2) && is_space(ptr[2]))
		set_hy(data, ptr + 2, i);
	else if (!ft_strncmp("tr", ptr, 2) && is_space(ptr[2]))
		set_tr(data, ptr + 2, i);
	else if (!ft_strncmp("co", ptr, 2) && is_space(ptr[2]))
		set_co(data, ptr + 2, i);
	else if (!ft_strncmp(ptr, "ply", 3) && is_space(ptr[3]))
		set_ply(data, ptr + 3, i);
	else if (*ptr == '#')
		return ;
	else
		clean_exit(data, EXIT_FAILURE, "Wrong identifier\n", i);
}

char	*jump_to_next_line(char *str)
{
	while (*str && *str != '\n')
		str++;
	if (*str == '\n')
		str++;
	return (str);
}

bool	verify_filename(char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 3)
		return (false);
	if (!ft_strncmp(filename + len - 3, ".rt",3))
		return (true);
	return (false);
}

void	read_rt(t_data *data)
{
	struct stat	file_stat;
	char		*cursor;
	char		*line_start;
	int			i;

	if (verify_filename(data->filename) == false)
		clean_exit(data, 1, "Error: File name\n", 0);
	data->scene_fd = open(data->filename, O_RDONLY);
	if (data->scene_fd < 0)
		clean_exit(data, 1, "Error: Open failed\n", 0);
	if (fstat(data->scene_fd, &file_stat) < 0)
		clean_exit(data, 1, "Error: Fstat failed\n", 0);
	if (S_ISDIR(file_stat.st_mode))
		clean_exit(data, 1, "Error: Is a directory\n", 0);
	data->scene_line = malloc(sizeof(char) * (file_stat.st_size + 1));
	if (!data->scene_line)
		clean_exit(data, 1, "Error: Malloc failed\n", 0);
	if (read(data->scene_fd, data->scene_line, file_stat.st_size) == -1)
		clean_exit(data, 1, "Error: Read failed\n", 0);
	data->scene_line[file_stat.st_size] = '\0';
	close(data->scene_fd);
	data->scene_fd = -1;
	i = 1;
	cursor = data->scene_line;
	while (*cursor)
	{
		line_start = cursor;
		while (*cursor && *cursor != '\n')
			cursor++;
		if (*cursor == '\n')
		{
			*cursor = '\0';
			cursor++;
		}
		char *ptr = line_start;
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
