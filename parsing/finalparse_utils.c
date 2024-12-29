/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalparse_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 14:47:21 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 14:48:58 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	handle_empty_node(t_line **node)
{
	(*node) = (*node)->next;
	return (1);
}

int	handle_pipe(t_line **node, t_cmd **new)
{
	(*new)->argv[0] = ft_strdup((*node)->str[0]);
	(*new)->argv[1] = NULL;
	(*node) = (*node)->next;
	return (1);
}

int	handle_redir(t_line **node, t_cmd **new)
{
	(*new)->argv[0] = ft_strdup((*node)->str[0]);
	(*node) = (*node)->next;
	(*new)->argv[1] = ft_strdup((*node)->str[0]);
	(*node) = (*node)->next;
	(*new)->argv[2] = NULL;
	return (1);
}
