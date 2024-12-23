/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:19:52 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/19 12:09:11 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_redirection(int type)
{
	if (type == REDIR_IN || type == REDIR_OUT || type == APPEND)
		return (1);
	return (0);
}

void	handle_read_from(t_cmd *cmd, t_data *data)
{
	if (!init_read_from(cmd, data))
	{
		while (cmd->prev && is_redirection(cmd->type))
		{
			cmd->file_error = 0;
			cmd = cmd->prev;
		}
		cmd->file_error = 0;
	}
}

void	handle_write_to(t_cmd *cmd, t_data *data)
{
	if (!init_write_to(cmd, data))
	{
		while (cmd->next && is_redirection(cmd->type))
		{
			cmd->file_error = 0;
			cmd = cmd->next;
		}	
		cmd->file_error = 0;
	}
}

void	handle_append(t_cmd *cmd, t_data *data)
{
	if (!init_append(cmd, data))
	{
		while (cmd->next && is_redirection(cmd->type))
		{
			cmd->file_error = 0;
			cmd = cmd->next;
		}
		cmd->file_error = 0;
	}
}

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
			handle_write_to(cmd, data);
		else if (cmd->type == REDIR_IN)
			handle_read_from(cmd, data);
		else if (cmd->type == APPEND)
			handle_append(cmd, data);
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

int	close_file(t_data *data, t_cmd *cmd)
{
	pid_t	wpid;
	int		status;

	close_fds(cmd, false);
	wpid = 0;
	while (wpid != -1 || errno != ECHILD)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == data->pid)
			exit_status = WEXITSTATUS(status);
	}
	return (0);
}

bool	remove_old_file_ref(t_io_fds *io, bool infile)
{
	if (!io)
		return (false);
	if (infile == true && io->infile)
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
		if (io->in_fd != -1)
			close(io->in_fd);
	}
	else if (infile == false && io->outfile)
	{
		if (io->out_fd == -1 || (io->infile && io->in_fd == -1))
			return (false);
		if (io->outfile)
		{
			printf("Freeing outfile %s\n", io->outfile);
			free(io->outfile);
			io->outfile = NULL;
		}
		if (io->out_fd != -1)
			close(io->out_fd);
	}
	return (true);
}
