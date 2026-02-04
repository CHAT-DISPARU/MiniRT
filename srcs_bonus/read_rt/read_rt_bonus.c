/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_rt_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 22:04:41 by titan             #+#    #+#             */
/*   Updated: 2026/02/04 12:29:14 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

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
	else
		clean_exit(data, EXIT_FAILURE, "Wrong identifier\n", i);
}

void	read_rt(t_data *data, char *filename)
{
	int		i;
	char	buffer[1];
	char	*ptr;

	i = 1;
	data->scene_fd = open(filename, O_RDONLY);
	if (data->scene_fd < 0)
		clean_exit(data, 1, "open fail\n", 0);
	if (read(data->scene_fd, buffer, 0) == -1)
		clean_exit(data, 1, "Error: The file is a directory\n", 0);
	data->scene_line = get_next_line(data->scene_fd, 0);
	while (data->scene_line)
	{
		ptr = data->scene_line;
		while (*ptr && is_space(*ptr))
			ptr++;
		if (*ptr && *ptr != '\n')
			call_setters(data, ptr, i);
		free(data->scene_line);
		data->scene_line = get_next_line(data->scene_fd, 0);
		i++;
	}
	close(data->scene_fd);
	data->scene_fd = -1;
}
