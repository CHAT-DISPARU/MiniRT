/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_inverse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 16:08:44 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/01 15:59:23 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib_mat.h>

/*
mat4 [y][x] -> mat4 [x][y]
*/

t_mat4	mat4_transpose(t_mat4 *m)
{
	t_mat4	res;
	int		i;
	int		j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			res.m[i][j] = m->m[j][i];
			j++;
		}
		i++;
	}
	return (res);
}

/*
	regle sarrus le chauve
	det(Mat3​)=a(ei−fh)−b(di−fg)+c(dh−eg)
*/

static double	det3x3(t_mat4 *m, int c1, int c2, int c3)
{
	return (m->m[1][c1] * (m->m[2][c2] * m->m[3][c3] - m->m[3][c2]
		* m->m[2][c3])
		- m->m[2][c1] * (m->m[1][c2] * m->m[3][c3] - m->m[3][c2] * m->m[1][c3])
		+ m->m[3][c1]
		* (m->m[1][c2] * m->m[2][c3] - m->m[2][c2] * m->m[1][c3]));
}

/*
	calcul det formule : a*Det(A) - b*Det(B) + c*Det(C) - d*Det(D) = det total
*/

double	mat4_deter(t_mat4 *m)
{
	double	det;

	det = 0.0f;
	det += m->m[0][0] * det3x3(m, 1, 2, 3);
	det -= m->m[0][1] * det3x3(m, 0, 2, 3);
	det += m->m[0][2] * det3x3(m, 0, 1, 3);
	det -= m->m[0][3] * det3x3(m, 0, 1, 2);
	return (det);
}

/*
	Matrice-¹= 1 / Determinant​ × Transposee(matrice de cofacteurs)
*/
static double	get_minor(t_mat4 *m, int skip_r, int skip_c)
{
	int			r[3];
	int			c[3];
	t_2index	indexs;

	indexs.j = 0;
	indexs.i = 0;
	while (indexs.i < 4)
	{
		if (indexs.i != skip_r)
			r[indexs.j++] = indexs.i;
		indexs.i++;
	}
	indexs.j = 0;
	indexs.i = 0;
	while (indexs.i < 4)
	{
		if (indexs.i != skip_c)
			c[indexs.j++] = indexs.i;
		indexs.i++;
	}
	return (m->m[r[0]][c[0]] * (m->m[r[1]][c[1]] * m->m[r[2]][c[2]]
		- m->m[r[2]][c[1]] * m->m[r[1]][c[2]]) - m->m[r[0]][c[1]]
		* (m->m[r[1]][c[0]] * m->m[r[2]][c[2]] - m->m[r[2]][c[0]]
		* m->m[r[1]][c[2]]) + m->m[r[0]][c[2]] * (m->m[r[1]][c[0]]
		* m->m[r[2]][c[1]] - m->m[r[2]][c[0]] * m->m[r[1]][c[1]]));
}

/*
	|​+−+−|
	|−+−+|
	|​+−+−|
	|​−+−+|​
*/

t_mat4	mat4_inverse(t_mat4 *m)
{
	t_mat4		res;
	double		det;
	double		inv_det;
	t_2index	indexs;

	det = mat4_deter(m);
	if (det == 0.0)
		return ((t_mat4){0});
	inv_det = 1.0 / det;
	indexs.i = 0;
	while (indexs.i < 4)
	{
		indexs.j = 0;
		while (indexs.j < 4)
		{
			res.m[indexs.j][indexs.i] = get_minor(m, indexs.i, indexs.j)
				* inv_det;
			if ((indexs.i + indexs.j) % 2 != 0)
				res.m[indexs.j][indexs.i] = -res.m[indexs.j][indexs.i];
			indexs.j++;
		}
		indexs.i++;
	}
	return (res);
}
