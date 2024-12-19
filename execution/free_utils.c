/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:37:02 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/19 12:27:12 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_cmd_node(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->argv)
			free_arr(cmd->argv);
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->pipe_fd)
			free(cmd->pipe_fd);
		if (cmd->io_fds)
			free(cmd->io_fds);
		free_io(cmd);
		free(cmd);
	}
}
