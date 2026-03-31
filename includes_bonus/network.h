/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:15:30 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/31 19:11:01 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_H
# define NETWORK_H

# include <stdint.h>
# include "minirt_bonus.h"
# define MSG_SCENE 1
# define MSG_TASK  2
# define MSG_PIXELS 3
# define MSG_END_TASKS 4

typedef struct s_net_texture
{
    int	width;
    int	height;
    int	scale;
    int	name_len;
}				t_net_texture;

typedef struct s_net_header
{
	uint8_t		type;
	uint32_t	size;
} __attribute__((packed)) t_net_header; 

typedef struct s_net_obj
{
	double		rad_1;
	double		rad_2;
	double		height;
	t_vec3		ka;
	double		ns;
	double		ni;
	double		opacity;
	t_vec3		kd;
	t_vec3		ks;
	int			type;
	t_triangle	tri;
	t_mat4		transform;
	t_mat4		inverse_transform;
	mlx_color	color;
	double		reflectivity;
	double		rought;
	bool		has_texture;
	bool		has_bump;
} t_net_obj;

typedef struct s_net_scene_base
{
	t_camera	cam;
	t_view_p	view_port;
	t_alight	alight;
	t_vec3		a_final;
	int			width;
	int			height;
	int			obj_count;
	int			bvh_node_count;
	int			light_count;
	int			s_per_pixs;
	int			deph;
	bool		use_bvh;
	int			plane_count;
	int			step;
	bool		debug;
	int			debug_depth;
	bool		diff_ok;
	bool		has_checker;
	mlx_color	checker_color;
}				t_net_scene_base;

typedef struct s_net_task
{
	uint32_t	task_id;
	int			start_x;
	int			end_x;
	int			start_y;
	int			end_y;
} t_net_task;

typedef struct s_net_result
{
	uint32_t	task_id;
	int			start_x;
	int			start_y;
	int			width;
	int			height;
}				t_net_result;

int		send_all(int socket, const void *buffer, size_t size);
int		recv_all(int socket, void *buffer, size_t size);
void	init_server(t_data *data);
void	check_new_clients(t_data *data);
void	run_worker(t_data *data, char *master_ip);
int		send_full_scene(int client_sock, t_data *data);
void	recv_task_result(int sock, t_data *data, uint32_t payload_size);

#endif
