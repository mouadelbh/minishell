/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalparse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:21 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/10/12 15:39:27 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
}

static int	size_to_alloc(t_line *node)
{
	int	i;
	int	size;

	size = 0;
	if (node->type != PIPE)
	{
		size++;
		node = node->next;
	}
	while (node && (node->type == 7 || node->type == 8))
	{
		i = 0;
		while (node->str[i])
		{
			i++;
			size++;
		}
		node = node->next;
	}
	return (size);
}

t_cmd	*get_current(t_line **node)
{
	t_cmd	*tmp;
	int		i;
	int		size;
	int		j;

	tmp = malloc(sizeof(t_cmd));
	if (!tmp)
		return (NULL);
	tmp->argv = malloc(sizeof(char *) * (size_to_alloc((*node)) + 1));
	i = 0;
	if ((*node)->type != PIPE)
	{
		tmp->argv[i++] = ft_strdup((*node)->str[0]);
		tmp->type = (*node)->type;
		(*node) = (*node)->next;
	}
	while ((*node) && ((*node)->type == 7 || (*node)->type == 8))
	{
		j = 0;
		while ((*node)->str[j])
			tmp->argv[i++] = ft_strdup((*node)->str[j++]);
		(*node) = (*node)->next;
	}
	tmp->argv[i] = NULL;
	tmp->next = NULL;
	return (tmp);
}

void	get_final_list(t_line **head, t_cmd **cmd)
{
	t_cmd	*new;
	t_line	*tmp;

	tmp = *head;
	new = NULL;
	while (tmp)
	{
		if (tmp->type == PIPE)
			tmp = tmp->next;
		if (!tmp)
			break ;
		new = get_current(&tmp);
		new->input_file = NULL;
		new->output_file = NULL;
		new->pipe_fd = NULL;
		lstadd_back(cmd, new);
	}
}
