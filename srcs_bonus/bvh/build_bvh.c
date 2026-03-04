/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_bvh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 13:46:56 by titan             #+#    #+#             */
/*   Updated: 2026/03/04 14:33:07 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	setleaf(t_data *data, int start, int count, t_bvh_node *node)
{
	node->left = -1;
	node->right = -1;
	node->start_idx = start;
	node->obj_count = count;
	node->debug_color = (t_vec3){0.0, 0.0, 1.0};
	data->proccessed_objs += count;
	if (data->proccessed_objs % 500 == 0)
		printf("\r%d/%d", data->proccessed_objs, data->obj_count);
}


int	recursive_build(t_data *data, int start, int count, int depth, t_vec3 branch_color)
{
	int			node_idx;
	t_bvh_node	*node;
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
		setleaf(data, start, count, node);
		return (node_idx);
	}
	node->obj_count = 0;
	mid = find_best_split_all_axes(data, start, count);
	if (mid <= 0 || mid >= count)
		mid = count / 2;
	node->left = recursive_build(data, start, mid, depth + 1, (t_vec3){0.0, 1.0, 0.0});
	node->right = recursive_build(data, start + mid, count - mid, depth + 1, (t_vec3){1.0, 0.0, 0.0});
	return (node_idx);
}

void	print_result(t_data *data)
{
	int	leaf_count;
	int	k;

	printf("\r%d/%d", data->proccessed_objs, data->obj_count);
	leaf_count = 0;
	k = 0;
	while (k < data->nodes_used)
	{
		if (data->bvh_nodes[k].left == -1)
			leaf_count++;
		k++;
	}
	printf("\nBVH finish :\n");
	printf("-Total nodes : %d\n", data->nodes_used);
	printf("-Total Objects : %d\n", data->obj_count);
	printf("-Total leafs : %d\n", leaf_count);
	if (leaf_count > 0)
		printf("- Moyenne Obj/leafs : %.2f\n", (double)data->obj_count / leaf_count);
	else
		printf("- Moyenne Obj/leafs : 0\n");
}

void	build_bvh(t_data *data)
{
	int	i;

	if (data->obj_count == 0)
		return;
	i = 0;
	data->proccessed_objs = 0;
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
	print_result(data);
}
