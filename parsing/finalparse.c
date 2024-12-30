/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalparse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:21 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 14:47:39 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static void	lstadd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

static int	size_to_alloc(t_line *node)
{
	int	i;
	int	size;

	size = 2;
	if (node->type == PIPE)
		return (size);
	if (isredir(node->type))
	{
		size += 2;
		return (size);
	}
	while (node && (node->type == CMD || node->type == ARG))
	{
		i = 0;
		while (node->str[i])
		{
			if (node->str[i][0])
				size++;
			i++;
		}
		node = node->next;
	}
	return (size);
}

static t_cmd	*copy_node(t_line *node)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->io_fds = NULL;
	new->argv = malloc(sizeof(char *) * size_to_alloc(node) + 1);
	new->pipe_output = 0;
	new->type = node->type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

int	get_current(t_line **node, t_cmd **new)
{
	int		i;
	int		j;

	if (empty_node(*node))
		return (handle_empty_node(node));
	(*new) = copy_node(*node);
	i = 0;
	j = 0;
	if ((*node)->type == PIPE)
		return (handle_pipe(node, new));
	else if (isredir((*node)->type))
		return (handle_redir(node, new));
	while ((*node) && ((*node)->type == CMD || (*node)->type == ARG))
	{
		j = 0;
		while ((*node)->str[j])
		{
			if ((*node)->str[j][0])
				(*new)->argv[i++] = ft_strdup((*node)->str[j]);
			j++;
		}
		(*node) = (*node)->next;
	}
	(*new)->argv[i] = NULL;
	return (0);
}

void	get_final_list(t_line **head, t_cmd **cmd)
{
	t_cmd	*new;
	t_line	*tmp;
	t_line	*newhead;

	tmp = NULL;
	new = NULL;
	arange_arguments(*head, &tmp);
	newhead = tmp;
	while (tmp)
	{
		get_current(&tmp, &new);
		lstadd_back(cmd, new);
	}
	free_line(newhead);
	split_command(cmd);
}
