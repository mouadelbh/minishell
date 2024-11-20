/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 02:55:16 by zelbassa          #+#    #+#             */
/*   Updated: 2024/11/20 13:10:06 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_io(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->io_fds)
		{
			if (cmd->io_fds->infile)
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
	if (data->envp_arr)
		free_arr(data->envp_arr);
	if (data->cmd_count != 0)
	{
		free_io(data->cmd);
	}
	free_line(&data->head);
	free_cmd_list(&data->cmd);
	free_cmd_struct(data->cmd);
	data->envp_arr = NULL;
}

void	free_data(t_data *data, int exit_code)
{
	free_all(data);
	// exit(exit_code);
}
