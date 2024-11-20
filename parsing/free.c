/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:25 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/20 15:22:27 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	ft_free(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		free(arg[i]);
		i++;
	}
	free(arg);
}

void	free_line(t_line **head)
{
	t_line	*tmp;
	int		i;

	i = 0;
	tmp = (*head);
	while (*head)
	{
		i = 0;
		while ((*head)->str[i])
			free((*head)->str[i++]);
		free((*head)->str);
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
}
