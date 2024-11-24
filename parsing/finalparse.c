/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalparse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:21 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/24 16:43:48 by mel-bouh         ###   ########.fr       */
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

	size = 1;
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
			if (!is_empty(node->str[i]))
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
	int		i;

	i = 0;
	new = malloc(sizeof(t_cmd));
	new->argv = malloc(sizeof(char *) * size_to_alloc(node));
	new->pipe_output = 0;
	new->type = node->type;
	new->next = NULL;
	new->prev = NULL;
}

int	empty_node(t_line *node)
{
	int	i;

	i = 0;
	while (node->str[i] && !node->str[i++][0]);
	if (!node->str[i])
		return (1);
	return (0);
}

void	get_current(t_line **node, t_cmd **new)
{
	int		i;
	int		j;

	if (!(*node)->str && empty_node(*node))
	{
		(*node) = (*node)->next;
		return ;
	}
	(*new) = copy_node(*node);
	i = 0;
	j = 0;
	if ((*node)->type == PIPE)
	{
		(*new)->argv[i++] = ft_strdup((*node)->str[0]);
		(*new)->argv[i] = NULL;
		(*node) = (*node)->next;
		return ;
	}
	else if (isredir((*node)->type))
	{
		(*new)->argv[i++] = ft_strdup(((*node)->str[0]));
		(*node) = (*node)->next;
		(*new)->argv[i++] = ft_strdup((*node)->str[0]);
		(*node) = (*node)->next;
		(*new)->argv[i] = NULL;
		return ;
	}
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
}

void	get_final_list(t_line **head, t_cmd **cmd)
{
	t_cmd	*new;
	t_line	*tmp;
	t_line	*newhead;
	int		i;

	tmp = NULL;
	new = NULL;
	arange_arguments(*head, &tmp);
	newhead = tmp;
	// while (newhead)
	// {
	// 	printf("this is a node\n");
	// 	printf("--------------\n");
	// 	for (int i = 0;newhead->str[i];i++)
	// 		printf("argv[%i]:%s\n", i, newhead->str[i]);
	// 	printf("type: %i\n", newhead->type);
	// 	newhead = newhead->next;
	// }
	while (tmp)
	{
		get_current(&tmp, &new);
		lstadd_back(cmd, new);
	}
	for (t_cmd *t = *cmd; t; t = t->next)
	{
		printf("this is a node\n");
		printf("--------------\n");
		for (int i = 0;t->argv[i];i++)
			printf("argv[%i]:%s\n", i, t->argv[i]);
		printf("type: %i\n", t->type);
	}
	free_line(&newhead);
	split_command(cmd);
	printf("%p\n", cmd);
}
