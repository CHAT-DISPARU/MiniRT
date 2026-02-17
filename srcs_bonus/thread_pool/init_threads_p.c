/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads_p.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 11:32:20 by gajanvie          #+#    #+#             */
/*   Updated: 2026/02/17 12:40:36 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	ft_lstdelone_stack(t_thread_p	*lst)
{
	if (lst)
		free(lst);
	lst = lst->next;
}

void	ft_lstclear_stack(t_thread_p **lst)
{
	t_thread_p	*current;
	t_thread_p	*tmp;

	if (lst == NULL || !*lst)
		return ;
	current = *lst;
	while (current)
	{
		tmp = current->next;
		ft_lstdelone(current);
		current = tmp;
	}
	*lst = NULL;
}

void	*routine(void	*arg)
{
	t_data			*data;
	t_thread_info	info;
	t_thread_p		*stack;
	t_task			function;
	bool			run;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->mutex_stack);
		if (!data->thread_running)
		{
			pthread_mutex_unlock(&data->mutex_stack);
			break ;
		}
		stack = data->stack;
		if (stack)
		{
			info = stack->info;
			function = stack->task;
			ft_lstdelone_stack(data->stack);
			function(&info);
		}
		pthread_mutex_unlock(&data->mutex_stack);
	}
}

void	stop_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < THREADS_COUNT)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&data->mutex_stack);
	ft_lstclear_stack(&data->stack);
}

void	init_thread_p(t_data *data)
{
	int				i;

	pthread_mutex_init(&data->mutex_stack, NULL);
	while (i < THREADS_COUNT)
	{
		if (pthread_create(&data->threads[i], NULL, routine, data))
			clean_exit(data, 1, "phtread create error\n", 0);
		i++;
	}
}
