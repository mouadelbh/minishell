/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalparse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:21 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/25 15:46:03 by mel-bouh         ###   ########.fr       */
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
	new->prev = tmp; // Set the previous pointer
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
			if (is_empty(node->str[i]))
				size--;
			i++;
			size++;
		}
		node = node->next;
	}
	return (size);
}

char	**get_splited(char **argv, int len)
{
	char	**str;
	char	**splited;
	int		i;
	int		j;

	i = 0;
	while (argv[i])
		i++;
	str = malloc(sizeof(char *) * (i + len));
	if (!str)
		return (NULL);
	i = 0;
	splited = ft_split(argv[0], ' ');
	if (!splited)
		return (NULL);
	while (splited[i])
	{
		str[i] = ft_strdup(splited[i]);
		free(splited[i++]);
	}
	j = 1;
	while (argv[j])
		str[i++] = ft_strdup(argv[j++]);
	str[i] = NULL;
	return (free(splited), str);
}

void	split_command(t_cmd **node)
{
	t_cmd	*tmp;
	char	**new;
	int		len;

	tmp = *node;
	while (tmp)
	{
		len = count_words(tmp->argv[0], ' ');
		if (tmp->type == CMD && len > 1)
		{
			new = get_splited(tmp->argv, len);
			ft_free(tmp->argv);
			tmp->argv = new;
		}
		tmp = tmp->next;
	}
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
	if ((*node)->type != PIPE && !is_empty((*node)->str[0]))
	{
		tmp->argv[i++] = ft_strdup((*node)->str[0]);
		tmp->type = (*node)->type;
		(*node) = (*node)->next;
	}
	while (*node && ((*node)->type == 7 || (*node)->type == 8))
	{
		j = 0;
		while ((*node)->str[j])
		{
			while ((*node)->str[j] && is_empty((*node)->str[j]))
				j++;
			if (!(*node)->str[j])
				break ;
			tmp->argv[i++] = ft_strdup((*node)->str[j++]);
		}
		(*node) = (*node)->next;
	}
	tmp->pipe_output = 0;
	tmp->argv[i] = NULL;
	tmp->next = NULL;
	tmp->prev = NULL; // Initialize the previous pointer to NULL
	return (tmp);
}

void	skip_to_arg(t_line **save)
{
	*save = (*save)->next;
	if (*save)
		*save = (*save)->next;
}

// void    switch_nodes(t_line **head, t_line *save, int cmd)
// {

// }

t_line	*copy_node(t_line *node)
{
	t_line	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(t_line));
	new->str = malloc(sizeof(node->str) + sizeof(char *));
	while (node->str[i])
	{
		new->str[i] = ft_strdup(node->str[i]);
		i++;
	}
	new->str[i] = NULL;
	new->type = node->type;
	new->data = node->data;
	new->next = NULL;
	new->prev = NULL;
}

// void	arange_arguments(t_line *head, t_line **new)
// {
// 	t_line	*node;
// 	t_line	*save;
// 	int		cmd;

// 	cmd = 0;
// 	while (head)
// 	{
// 		if ("")
// 		head = head->next;
// 	}
// }

void	get_final_list(t_line **head, t_cmd **cmd)
{
	t_cmd	*new;
	t_line	*tmp;
	int		i;

	new = NULL;
	tmp = *head;
	// arange_arguments(head, &tmp);
	while (tmp)
	{
		if (tmp->type == PIPE)
			tmp = tmp->next;
		if (!tmp)
			break ;
		new = get_current(&tmp);
		lstadd_back(cmd, new);
	}
	split_command(cmd);
}
