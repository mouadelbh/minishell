/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 02:55:16 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/23 10:27:58 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_io(t_cmd *cmd)
{
	while (cmd && cmd->io_fds)
	{
		if (cmd->io_fds->infile)
		{
			if (cmd->io_fds->infile)
				free(cmd->io_fds->infile);
			cmd->io_fds->infile = NULL;
			close(cmd->io_fds->in_fd);
		}
		if (cmd->io_fds->outfile)
		{
			if (cmd->io_fds->outfile)
				free(cmd->io_fds->outfile);
			cmd->io_fds->outfile = NULL;
			close(cmd->io_fds->out_fd);
		}
		if (cmd->io_fds->heredoc_name)
		{
			unlink(cmd->io_fds->heredoc_name);
			free(cmd->io_fds->heredoc_name);
		}
		free(cmd->io_fds);
		cmd = cmd->next;
	}
}

void	free_cmd_node(t_cmd *cmd)
{
	if (cmd->argv)
		free_arr(cmd->argv);
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	free(cmd);
	cmd = NULL;
}

void	free_cmd_struct(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (cmd && cmd->io_fds)
		free_io(cmd);
	while (cmd)
	{
		tmp = cmd->next;
		free_cmd_node(cmd);
		cmd = tmp;
	}
}

void	free_env(t_list **envp)
{
	t_list	*tmp;

	while (*envp)
	{
		tmp = (*envp)->next;
		free((*envp)->content);
		free(*envp);
		*envp = tmp;
	}
}

void	free_all(t_data *data, int i)
{
	t_cmd	*cmd;

	cmd = data->cmd;
	if (i)
		free_env(&data->envp);
	if (data->envp_arr)
	{
		free_arr(data->envp_arr);
		data->envp_arr = NULL;
	}
	free_cmd_struct(cmd);
	free_line(data->head);
}

void	free_data(t_data *data, int exit_code)
{
	free_all(data, 1);
}
