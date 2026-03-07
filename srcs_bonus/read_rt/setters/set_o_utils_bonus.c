/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_o_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 16:25:28 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/07 16:28:22 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

t_vec3	parse_vec_fast(char **s, int type)
{
	t_vec3	p;

	p.x = parse_double(s, 0);
	p.y = parse_double(s, 0);
	if (type == 1)
		p.z = parse_double(s, 0);
	else
		p.z = 0;
	return (p);
}

char	*ft_strncat(char *dest, char *src, unsigned int nb)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i ++;
	while (j < nb && src[j] != '\0')
	{
		dest[i + j] = src[j];
		j ++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*map_file_fast(char *filename, size_t *size)
{
	int			fd;
	struct stat	st;
	char		*ptr;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	if (fstat(fd, &st) < 0)
	{
		close(fd);
		return (NULL);
	}
	*size = st.st_size;
	ptr = mmap(NULL, *size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED)
		return (NULL);
	return (ptr);
}

void	do_firstpart_double(char **s, double *res, double *sign)
{
	*sign = 1;
	skip_spaces(s);
	if (**s == '-')
	{
		*sign = -1.0;
		(*s)++;
	}
	while (**s >= '0' && **s <= '9')
	{
		*res = *res * 10.0 + (**s - '0');
		(*s)++;
	}
}

double	parse_double(char **s, double res)
{
	double		sign;
	long long	fraction;
	long long	divisor;
	bool		has_fraction;

	res = 0;
	fraction = 0;
	divisor = 1;
	has_fraction = false;
	do_firstpart_double(s, &res, &sign);
	if (**s == '.')
	{
		(*s)++;
		while (**s >= '0' && **s <= '9')
		{
			fraction = fraction * 10 + (**s - '0');
			divisor *= 10;
			(*s)++;
			has_fraction = true;
		}
	}
	if (has_fraction)
		res += (double)fraction / (double)divisor;
	return (res * sign);
}
