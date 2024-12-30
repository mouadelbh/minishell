/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:24:51 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/27 13:53:15 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*set_new(t_cmd *new, t_cmd *src, int i)
{
	new->argv = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (src->argv[i])
	{
		new->argv[i] = ft_strdup(src->argv[i]);
		i++;
	}
	new->argv[i] = NULL;
	new->cmd = ft_strdup(src->cmd);
	new->type = src->type;
	new->pipe_fd[0] = src->pipe_fd[0];
	new->pipe_fd[1] = src->pipe_fd[1];
	new->pipe_output = src->pipe_output;
	new->io_fds = dup_io(src->io_fds);
	new->file_error = src->file_error;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_cmd	*copy_node(t_cmd *src)
{
	t_cmd	*new;
	int		i;

	i = 0;
	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	while (src->argv[i])
		i++;
	return (set_new(new, src, i));
}

void	free_tmp_io(t_io_fds *io)
{
	if (io->infile)
	{
		free(io->infile);
		io->infile = NULL;
	}
	if (io->outfile)
	{
		free(io->outfile);
		io->outfile = NULL;
	}
	if (io->heredoc_name)
	{
		unlink(io->heredoc_name);
		free(io->heredoc_name);
		io->heredoc_name = NULL;
	}
	free(io);
}

void	set_cmd_list(t_cmd **cmd, t_cmd **new)
{
	t_cmd	*tmp;
	t_cmd	*current;

	tmp = *cmd;
	while (tmp)
	{
		current = tmp->next;
		if (tmp->type == CMD)
			lstadd_cmd(new, copy_node(tmp));
		if (tmp->io_fds)
			free_tmp_io(tmp->io_fds);
		free_cmd_node(tmp);
		tmp = current;
	}
}

int	complex_command(t_data *data)
{
	t_cmd	*new;

	new = NULL;
	if (data->cmd)
	{
		if (!create_files(data->cmd, data))
		{
			if (g_exit_status > 127)
				return (g_exit_status);
			return (1);
		}
		if (!create_pipes(data))
		{
			ft_putstr_fd("Failed to create pipes\n", 2);
			close_pipe_fds(data->cmd);
			return (EXIT_FAILURE);
		}
		if (set_values(data) == 1)
			return (1);
		set_cmd_list(&data->cmd, &new);
		data->cmd = new;
		return (handle_execute(data));
	}
	return (0);
}
