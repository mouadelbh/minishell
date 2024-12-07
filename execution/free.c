/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 02:55:16 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/05 13:39:26 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_io(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->io_fds != NULL)
		{
			if (cmd->io_fds && cmd->io_fds->infile)
				free(cmd->io_fds->infile);
			if (cmd->io_fds->outfile)
				free(cmd->io_fds->outfile);
			if (cmd->io_fds->heredoc_name)
				free(cmd->io_fds->heredoc_name);
			if (cmd->pipe_output)
			{
				free(cmd->pipe_fd);
				cmd->pipe_fd = NULL;
			}
			free(cmd->io_fds);
			cmd->io_fds = NULL;
		}
		cmd = cmd->next;
	}
}

static void	free_cmd_struct(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->argv)
			free_arr(cmd->argv);
		free(cmd);
		cmd = tmp;
	}
}

void	free_all(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmd;
	if (data->envp_arr)
		free_arr(data->envp_arr);
	// if (cmd->io_fds)
	// 	free_io(cmd);
	free_io(cmd);
	free_line(data->head);
	free_cmd_struct(cmd);
	data->envp_arr = NULL;
}

static void	free_env(t_list **envp)
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

void	free_data(t_data *data, int exit_code)
{
	free_all(data);
	free_env(&data->envp);
}
