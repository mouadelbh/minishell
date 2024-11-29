/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrange.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:07:46 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/29 17:45:32 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static t_line	*copy_node(t_line *node)
{
	t_line	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(t_line));
	while (node->str[i])
		i++;
	new->str = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (node->str[i])
	{
		new->str[i] = ft_strdup(node->str[i]);
		i++;
	}
	new->str[i] = NULL;
	new->type = node->type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	get_arranged(t_line **current, t_line **new)
{
	t_line	*save;
	t_line	*node;
	int		cmd;

	cmd = 0;
	save = NULL;
	if ((*current)->type == CMD)
		cmd = 1;
	node = *current;
	while (node)
	{
		if (isredir((node)->type))
		{
			lstadd_line(&save, copy_node((node)));
			node = (node)->next;
			if (node)
			{
				lstadd_line(&save, copy_node((node)));
				node = (node)->next;
			}
			if (node && cmd == 1 && (node)->type == CMD)
				(node)->type = ARG;
		}
		else
		{
			lstadd_line(new, copy_node(node));
			node = (node)->next;
		}
	}
	node = save;
	while (node)
	{
		lstadd_line(new, copy_node(node));
		node = node->next;
	}
	free_line(save);
}

void	arange_arguments(t_line *head, t_line **final)
{
	t_line	*current;
	t_line	*new;
	t_line	*node;

	while (head)
	{
		current = NULL;
		new = NULL;
		while (head && head->type != PIPE)
		{
			lstadd_line(&current, copy_node(head));
			head = head->next;
		}
		get_arranged(&current, &new);
		free_line(current);
		node = new;
		while (node)
		{
			lstadd_line(final, copy_node(node));
			node = node->next;
		}
		free_line(new);
		if (head && head->type == PIPE)
		{
			lstadd_line(final, copy_node(head));
			head = head->next;
		}
	}
}
