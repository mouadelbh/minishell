/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ios.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:15:25 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/26 15:19:46 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_fds(t_cmd *cmds)
{
	if (cmds && cmds->io_fds)
	{
		ft_close(cmds->io_fds->in_fd);
		ft_close(cmds->io_fds->out_fd);
	}
	close_pipe_fds(cmds);
}

void	print_io_error(int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			ft_putstr_fd("dup2 error\n", 2);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			ft_putstr_fd("dup2 error\n", 2);
		close(out_fd);
	}
}

bool	redirect_io(t_io_fds *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	print_io_error(io->in_fd, io->out_fd);
	return (ret);
}

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}
