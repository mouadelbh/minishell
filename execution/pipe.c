/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:17:37 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/30 18:44:07 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	create_pipes(t_data *data)
{
	int		i;
	t_cmd	*cmd;

	i = 1;
	cmd = data->cmd;
	while (cmd)
	{
		if (cmd->pipe_output)
		{
			if (pipe(cmd->pipe_fd) != 0)
			{
				if (errno == EMFILE)
				{
					i = 0;
					close_pipe_fds(data->cmd);
				}
				break ;
			}
		}
		cmd = cmd->next;
	}
	return (i);
}

void	close_pipe_fds(t_cmd *cmds)
{
	while (cmds && cmds->pipe_output)
	{
		ft_close(cmds->pipe_fd[0]);
		ft_close(cmds->pipe_fd[1]);
		cmds = cmds->next;
	}
}

bool	set_pipe_fds(t_cmd *c)
{
	if (!c)
		return (false);
	if (c->prev && c->prev->pipe_output)
		dup2(c->prev->pipe_fd[0], STDIN_FILENO);
	if (c->pipe_output)
		dup2(c->pipe_fd[1], STDOUT_FILENO);
	return (true);
}
