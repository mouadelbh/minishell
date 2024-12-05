/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:36:32 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/04 15:41:21 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	set_cmd_node(t_cmd *cmd, t_cmd *current, t_cmd *temp, t_cmd *new_list)
{
	if (cmd->type == CMD)
	{
		temp = init_new_cmd(cmd);
		if (!temp)
		{
			while (new_list)
			{
				temp = new_list->next;
				free(new_list);
				new_list = temp;
			}
			return (1);
		}
		current->next = temp;
		temp->prev = current;
		current = temp;
	}
	return (0);
}

t_cmd	*set_command_list(t_cmd *cmd)
{
	t_cmd	*new_list;
	t_cmd	*current;
	t_cmd	*temp;

	if (!cmd)
		return (NULL);
	while (cmd && cmd->type != CMD && cmd->next)
	{
		free_cmd_node(cmd);
		cmd = cmd->next;
	}
	if (!cmd)
		return (NULL);
	new_list = init_new_cmd(cmd);
	if (!new_list)
		return (NULL);
	current = new_list;
	cmd = cmd->next;
	while (cmd)
	{
		if (set_cmd_node(cmd, current, temp, new_list))
			return (NULL);
		cmd = cmd->next;
	}
	return (new_list);
}

t_cmd	*init_new_cmd(t_cmd *src)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = src->argv;
	new->cmd = src->cmd;
	new->type = src->type;
	new->pipe_fd = src->pipe_fd;
	new->pipe_output = src->pipe_output;
	new->io_fds = src->io_fds;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
