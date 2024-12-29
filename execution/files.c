/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:19:52 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 18:18:13 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_files(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 1;
	while (cmd)
	{
		init_io(&cmd->io_fds);
		if (cmd->type == CMD)
			cmd->file_error = init_command(cmd, data);
		else if (cmd->type == REDIR_OUT)
			handle_write_to(cmd);
		else if (cmd->type == REDIR_IN)
			handle_read_from(cmd);
		else if (cmd->type == APPEND)
			handle_append(cmd);
		else if (cmd->type == HEREDOC)
			cmd->file_error = init_heredoc(cmd, data);
		i = cmd->file_error;
		cmd = cmd->next;
	}
	return (i);
}

bool	check_infile_outfile(t_io_fds *io)
{
	if ((io->infile && io->in_fd == -1)
		|| (io->outfile && io->out_fd == -1))
		return (false);
	if (!io || (!io->infile && !io->outfile))
		return (true);
	return (true);
}

bool	remove_outfile_ref(t_io_fds *io)
{
	if (io->out_fd == -1 || (io->infile && io->in_fd == -1))
		return (false);
	if (io->outfile)
	{
		free(io->outfile);
		io->outfile = NULL;
	}
	ft_close(io->out_fd);
	return (true);
}

bool	remove_infile_ref(t_io_fds *io)
{
	if (io->in_fd == -1 || (io->outfile && io->out_fd == -1))
		return (false);
	if (io->heredoc_name != NULL)
	{
		free(io->heredoc_name);
		io->heredoc_name = NULL;
		unlink(io->infile);
	}
	if (io->infile)
	{
		free(io->infile);
		io->infile = NULL;
	}
	ft_close(io->in_fd);
	return (true);
}

bool	remove_old_file_ref(t_io_fds *io, bool infile)
{
	if (!io)
		return (false);
	if (infile == true && io->infile)
		return (remove_infile_ref(io));
	else if (infile == false && io->outfile)
		return (remove_outfile_ref(io));
	return (true);
}
