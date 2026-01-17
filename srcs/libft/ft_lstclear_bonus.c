/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:34:42 by gajanvie          #+#    #+#             */
/*   Updated: 2025/11/04 18:44:18 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst)
{
	t_list	*current;
	t_list	*tmp;

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
