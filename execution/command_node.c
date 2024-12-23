/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:36:32 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/23 13:37:06 by zelbassa         ###   ########.fr       */
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
	t_cmd	*new_list;
	t_cmd	*current;
	t_cmd	*temp;
	t_cmd	*next_cmd;

	if (!cmd)
		return (NULL);
	while (cmd && (cmd->type != CMD))
	{
		t_cmd *next_cmd = cmd->next;
		show_command_info(cmd);
		if (cmd->io_fds)
			free_io(cmd);
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
			// if (!temp)
			// {
			// 	while (new_list)
			// 	{
			// 		temp = new_list->next;
			// 		free(new_list);
			// 		new_list = temp;
			// 	}
			// 	return (NULL);
			// }
			current->next = temp;
			current = temp;
		}
		cmd = cmd->next;
	}
	return (new_list);
}

int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

int	*dupfd(int *i)
{
	int *new;

	new = (int *)malloc(sizeof(int) * 2);
	if (!new)
		return (NULL);
	new[0] = i[0];
	new[1] = i[1];
	return (new);
}

t_cmd	*init_new_cmd(t_cmd *src, t_cmd *next, t_cmd *prev)
{
	t_cmd	*new;
	int		i;

	i = 0;
	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	while (src->argv[i])
		i++;
	new->argv = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (src->argv[i])
	{
		new->argv[i] = ft_strdup(src->argv[i]);
		i++;
	}
	// new->argv = src->argv;
	new->cmd = ft_strdup(src->cmd);
	new->type = src->type;
	// new->pipe_fd = src->pipe_fd;
	new->pipe_output = src->pipe_output;
	new->io_fds = dup_io(src->io_fds);
	new->file_error = src->file_error;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
