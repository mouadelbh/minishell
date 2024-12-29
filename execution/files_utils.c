/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:10:23 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/27 14:09:55 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_redirection(int type)
{
	if (type == REDIR_IN || type == REDIR_OUT || type == APPEND)
		return (1);
	return (0);
}

void	handle_read_from(t_cmd *cmd)
{
	if (!init_read_from(cmd))
	{
		while (cmd->prev && is_redirection(cmd->type))
		{
			cmd->file_error = 0;
			cmd = cmd->prev;
		}
		cmd->file_error = 0;
	}
}

void	handle_write_to(t_cmd *cmd)
{
	if (!init_write_to(cmd))
	{
		while (cmd->prev && is_redirection(cmd->type))
		{
			cmd->file_error = 0;
			cmd = cmd->prev;
		}
		cmd->file_error = 0;
	}
}

void	handle_append(t_cmd *cmd)
{
	if (!init_append(cmd))
	{
		while (cmd->prev && is_redirection(cmd->type))
		{
			cmd->file_error = 0;
			cmd = cmd->prev;
		}
		cmd->file_error = 0;
	}
}
