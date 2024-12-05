/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:17:37 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/02 23:54:32 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	create_pipes(t_data *data)
{
	int			*fd;
	t_cmd		*tmp;

	tmp = data->cmd;
	while (tmp)
	{
		if (tmp->pipe_output)
		{
			fd = malloc(sizeof * fd * 2);
			if (!fd || pipe(fd) != 0)
				return (false);
			tmp->pipe_fd = fd;
		}
		tmp = tmp->next;
	}
	return (true);
}

void	close_pipe_fds(t_cmd *cmds, t_cmd *skip_cmd)
{
	while (cmds && cmds->pipe_output)
	{
		if (cmds->pipe_fd[0] != -1)
			close(cmds->pipe_fd[0]);
		if (cmds->pipe_fd[1] != -1)
			close(cmds->pipe_fd[1]);
		cmds = cmds->next;
	}
}

bool	set_pipe_fds(t_cmd *cmds, t_cmd *c)
{
	if (!c)
		return (false);
	if (c->prev && c->prev->pipe_output)
		dup2(c->prev->pipe_fd[0], STDIN_FILENO);
	if (c->pipe_output)
		dup2(c->pipe_fd[1], STDOUT_FILENO);
	close_pipe_fds(cmds, c);
	return (true);
}
