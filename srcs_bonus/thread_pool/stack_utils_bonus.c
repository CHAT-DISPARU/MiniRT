/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:58:25 by gajanvie          #+#    #+#             */
/*   Updated: 2026/03/11 10:59:49 by gajanvie         ###   ########.fr       */
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
