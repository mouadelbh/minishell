
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ios.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:17:09 by zelbassa          #+#    #+#             */
/*   Updated: 2024/10/22 12:25:19 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_fds(t_cmd *cmds, bool close_backups)
{
	if (cmds && cmds->io_fds)
	{
		if (cmds->io_fds->in_fd != -1)
			close(cmds->io_fds->in_fd);
		if (cmds->io_fds->out_fd != -1)
			close(cmds->io_fds->out_fd);
		if (close_backups)
			restore_io(cmds->io_fds);
	}
	close_pipe_fds(cmds);
}

bool	redirect_io(t_io_fds *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	io->stdin_backup = dup(STDIN_FILENO);
	if (io->stdin_backup == -1)
	{
		ft_putstr_fd("stdin backup error\n", 2);
		ret = 1;
	}
	io->stdout_backup = dup(STDOUT_FILENO);
	if (io->stdout_backup == -1)
	{
		ft_putstr_fd("stdout backup error\n", 2);
		ret = 1;
	}
	if (io->in_fd != -1)
	{
		if (dup2(io->in_fd, STDIN_FILENO) == -1)
			ft_putstr_fd("dup2 error\n", 2);
		close(io->in_fd);
	}
	if (io->out_fd != -1)
	{
		if (dup2(io->out_fd, STDOUT_FILENO) == -1)
			ft_putstr_fd("dup2 error\n", 2);
		close(io->out_fd);
	}
	return (ret);
}

bool	restore_io(t_io_fds *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	if (io->stdin_backup != -1)
	{
		if (dup2(io->stdin_backup, STDIN_FILENO) == -1)
			ret = false;
		close(io->stdin_backup);
		io->stdin_backup = -1;
	}
	if (io->stdout_backup != -1)
	{
		if (dup2(io->stdout_backup, STDOUT_FILENO) == -1)
			ret = false;
		close(io->stdout_backup);
		io->stdout_backup = -1;
	}
	return (ret);
}
