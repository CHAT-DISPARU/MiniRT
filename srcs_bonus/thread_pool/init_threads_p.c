/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads_p.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 11:32:20 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/10 13:51:40 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	ft_lstadd_front_stack(t_thread_p **lst, t_thread_p *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}

t_thread_p	*ft_lstnew_stack(t_thread_info content, t_task func)
{
	t_thread_p	*new_node;

	new_node = ft_calloc(1, sizeof(t_thread_p));
	if (new_node)
	{
		new_node->info = content;
		new_node->task = func;
	}
	return (new_node);
}

void	ft_lstdelone_stack(t_thread_p *lst)
{
	if (lst)
		free(lst);
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
		ft_lstdelone_stack(current);
		current = tmp;
	}
	*lst = NULL;
}

void	lunch_task(t_data *data)
{
	t_thread_info	info;
	t_task			function;
	t_thread_p		*tmp;

	info = data->stack->info;
	function = data->stack->task;
	tmp = data->stack;
	data->stack = data->stack->next;
	ft_lstdelone_stack(tmp);
	pthread_mutex_unlock(&data->mutex_stack);
	function(&info);
}

void	*routine(void	*arg)
{
	t_data			*data;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->mutex_stack);
		if (!data->thread_running)
		{
			pthread_mutex_unlock(&data->mutex_stack);
			break ;
		}
		if (data->stack)
			lunch_task(data);
		else
			pthread_mutex_unlock(&data->mutex_stack);
		usleep(100);
	}
	return (NULL);
}

void	add_task(t_data *data, t_task func, t_thread_info info)
{
	t_thread_p		*stack;

	stack = ft_lstnew_stack(info, func);
	if (!stack)
		clean_exit(data, 1, "Malloc\n", 0);
	pthread_mutex_lock(&data->mutex_stack);
	ft_lstadd_front_stack(&data->stack, stack);
	pthread_mutex_unlock(&data->mutex_stack);
}

void	stop_threads(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->mutex_stack);
	data->thread_running = false;
	pthread_mutex_unlock(&data->mutex_stack);
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
	int	i;

	i = 0;
	pthread_mutex_init(&data->mutex_stack, NULL);
	while (i < THREADS_COUNT)
	{
		if (pthread_create(&data->threads[i], NULL, routine, data))
			clean_exit(data, 1, "phtread create error\n", 0);
		i++;
	}
}
