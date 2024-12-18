/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:36:32 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/18 04:57:40 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	set_cmd_node(t_cmd *cmd, t_cmd *current, t_cmd *temp, t_cmd *new_list)
{
	if (cmd->type == CMD)
	{
		temp = init_new_cmd(cmd, cmd->next, cmd->prev);
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

t_cmd *set_command_list(t_cmd *cmd)
{
	t_cmd *new_list;
	t_cmd *current;
	t_cmd *temp;

	if (!cmd)
		return (NULL);
	while (cmd && (cmd->type != CMD))
	{
		t_cmd *next_cmd = cmd->next;
		free_cmd_node(cmd);
		cmd = next_cmd;
		if (!cmd)
			return (NULL);
	}
	if (!cmd)
		return (NULL);
	new_list = init_new_cmd(cmd, cmd->next, cmd->prev);
	if (!new_list)
		return (NULL);
	current = new_list;
	cmd = cmd->next;
	while (cmd)
	{
		if (cmd->type == CMD)
		{
			temp = init_new_cmd(cmd, cmd->next, current);
			if (!temp)
			{
				while (new_list)
				{
					temp = new_list->next;
					free(new_list);
					new_list = temp;
				}
				return (NULL);
			}
			current->next = temp;
			current = temp;
		}
		cmd = cmd->next;
	}
	return (new_list);
}

t_cmd	*init_new_cmd(t_cmd *src, t_cmd *next, t_cmd *prev)
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
	new->file_error = src->file_error;
	new->next = next;
	new->prev = prev;
	return (new);
}
