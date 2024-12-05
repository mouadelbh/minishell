/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:37:02 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/04 15:37:12 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_cmd_node(t_cmd *cmd)
{
	if (cmd->argv)
		free_arr(cmd->argv);
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	if (cmd->io_fds)
	{
		if (cmd->io_fds->infile)
			free(cmd->io_fds->infile);
		if (cmd->io_fds->outfile)
			free(cmd->io_fds->outfile);
		if (cmd->io_fds->heredoc_name)
			free(cmd->io_fds->heredoc_name);
		free(cmd->io_fds);
	}
	free(cmd);
	cmd = NULL;
}
