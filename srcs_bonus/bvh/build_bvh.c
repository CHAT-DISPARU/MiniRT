/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_bvh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: titan <titan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 13:46:56 by titan             #+#    #+#             */
/*   Updated: 2026/02/11 15:12:30 by titan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

int	recursive_build(t_data *data, int start, int count, int depth, t_vec3 branch_color)
{
	int			node_idx;
	t_bvh_node	*node;
	int			axis;
	int			mid;

	node_idx = data->nodes_used++;
	if (data->nodes_used >= data->nodes_capacity)
		clean_exit(data, 1, "Plus de place dans le tableau BVH !\n", 0);
	node = &data->bvh_nodes[node_idx];
	node->depth = depth;
	node->box = compute_bounds(&data->sorted_objs[start], count);
	node->debug_color = branch_color;
	if (count <= 1 || depth >= MAX_BVH_DEPTH)
	{
		node->left = -1;
		node->right = -1;
		node->start_idx = start;
		node->obj_count = count;
		node->debug_color = (t_vec3){0.0, 0.0, 1.0};
		return (node_idx);
	}
	node->obj_count = 0;
	t_vec3 size = vec_sub(node->box.max, node->box.min);
	if (size.x >= size.y && size.x >= size.z)
		axis = 0;
	else if (size.y >= size.z)
		axis = 1;
	else
		axis = 2;
	if (axis == 0)
		qsort(&data->sorted_objs[start], count, sizeof(t_obj *), cmp_x);
	else if (axis == 1)
		qsort(&data->sorted_objs[start], count, sizeof(t_obj *), cmp_y);
	else
		qsort(&data->sorted_objs[start], count, sizeof(t_obj *), cmp_z);
	mid = find_best_split_sah(data, start, count);
	node->left = recursive_build(data, start, mid, depth + 1, (t_vec3){0.0, 1.0, 0.0});
	node->right = recursive_build(data, start + mid, count - mid, depth + 1, (t_vec3){1.0, 0.0, 0.0});
	return (node_idx);
}

void	build_bvh(t_data *data)
{
	int i;
	int	leaf_count;
	int	k;

	if (data->obj_count == 0)
		return;
	i = 0;
	data->sorted_objs = malloc(sizeof(t_obj *) * data->obj_count);
	if (!data->sorted_objs)
		clean_exit(data, 1, "Malloc", 0);
	while (i < data->obj_count)
	{
		data->sorted_objs[i] = &data->array_obj[i];
		i++;
	}
	data->nodes_capacity = data->obj_count * 4;
	data->bvh_nodes = malloc(sizeof(t_bvh_node) * data->nodes_capacity);
	if (!data->bvh_nodes)
		clean_exit(data, 1, "Malloc", 0);
	data->nodes_used = 0;
	printf("Construction BVH (Max Depth: %d)...\n", MAX_BVH_DEPTH);
	recursive_build(data, 0, data->obj_count, 0, (t_vec3){1.0, 1.0, 0.0});
	leaf_count = 0;
	k = 0;
	while (k < data->nodes_used)
	{
		if (data->bvh_nodes[k].left == -1)
			leaf_count++;
		k++;
	}
	printf("BVH Terminé :\n");
	printf("- Total Noeuds : %d\n", data->nodes_used);
	printf("- Total Objets : %d\n", data->obj_count);
	printf("- Total Feuilles : %d\n", leaf_count);
	if (leaf_count > 0)
		printf("- Moyenne Obj/Feuille : %.2f\n", (double)data->obj_count / leaf_count);
	else
		printf("- Moyenne Obj/Feuille : 0\n");
}
