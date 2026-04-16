/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:18:36 by gajanvie          #+#    #+#             */
/*   Updated: 2026/04/16 15:30:04 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

#include <stdbool.h>

bool	is_ip_already_connected(t_data *data, struct sockaddr_in *new_client_addr)
{
	struct sockaddr_in	peer_addr;
	socklen_t			peer_len;
	int					i;

	peer_len = sizeof(peer_addr);
	i = 0;
	while (i < data->client_count)
	{
		if (data->client_sockets[i] != -1 && getpeername(data->client_sockets[i], (struct sockaddr *)&peer_addr, &peer_len) == 0)
		{
			if (peer_addr.sin_addr.s_addr == new_client_addr->sin_addr.s_addr)
				return (true);
		}
		i++;
	}
	return (false);
}

void	init_server(t_data *data)
{
	struct sockaddr_in	server_addr;
	int					opt = 1;

	data->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (data->server_socket < 0)
		clean_exit(data, 1, "Erreur creation socket\n", 0);
	setsockopt(data->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(data->server_socket, F_SETFL, O_NONBLOCK);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);
	if (bind(data->server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		clean_exit(data, 1, "Erreur bind socket\n", 0);
	if (listen(data->server_socket, 10) < 0)
		clean_exit(data, 1, "Erreur listen socket\n", 0);
	data->client_count = 0;
}

void	check_new_clients(t_data *data)
{
	int					new_sock;
	struct sockaddr_in	client_addr;
	socklen_t			addr_len = sizeof(client_addr);

	while (1)
	{
		new_sock = accept(data->server_socket, (struct sockaddr *)&client_addr, &addr_len);
		if (new_sock < 0)
			break ;
		#if BLOCK_SAME_IP == 1
		if (is_ip_already_connected(data, &client_addr))
		{
			printf("\nUn gros plot de chantier avec cette IP est deja connecte. Rejete gros plot vivant va te laver.\n");
			close(new_sock);
			continue ;
		}
		#endif
		if (data->client_count < CLIENT_MAX)
		{
			data->client_sockets[data->client_count] = new_sock;
			data->client_count++;
			printf("\nnouveau gros plot de chantier ! Total : %d\n", data->client_count);
			if (send_full_scene(data->client_sockets[data->client_count - 1], data) == -1)
				ft_putstr_fd("error scene envoie\n", 2);
		}
		else
		{
			printf("\nTrop de gros plot de chantiers.\n");
			close(new_sock);
		}
	}
}
