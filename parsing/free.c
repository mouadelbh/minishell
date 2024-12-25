/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:25 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 16:23:03 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr && !*arr)
		return ;
	while (arr[i])
	{
		if (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		i++;
	}
	free(arr);
	arr = NULL;
}

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

void	free_line(t_line *head)
{
	t_line	*tmp;
	int		i;

	i = 0;
	tmp = head;
	while (head)
	{
		i = 0;
		if (head->str)
		{
			while (head->str[i])
				free(head->str[i++]);
			free(head->str);
		}
		tmp = head->next;
		free(head);
		head = tmp;
	}
}
