/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:24:39 by zelbassa          #+#    #+#             */
/*   Updated: 2024/10/27 21:13:01 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_cmd(t_cmd *cmd)
{
	cmd->argv = NULL;
	cmd->cmd = NULL;
	cmd->pipe_fd = NULL;
	cmd->io_fds = NULL;
	cmd->type = 0;
	cmd->next = NULL;
	cmd->prev = NULL;
}

void	init_io(t_io_fds **io_fds)
{
	*io_fds = (t_io_fds *)malloc(sizeof(t_io_fds));
	if (!(*io_fds))
	{
		ft_putstr_fd("Failed to allocate memory\n", 2);
		return ;
	}
	(*io_fds)->in_fd = -1;
	(*io_fds)->out_fd = -1;
	(*io_fds)->infile = NULL;
	(*io_fds)->outfile = NULL;
	(*io_fds)->heredoc_name = NULL;
	(*io_fds)->stdin_backup = -1;
	(*io_fds)->stdout_backup = -1;
}

void init_write_to(t_cmd *cmd, t_data *data)
{
	init_io(&cmd->io_fds);
	if (!remove_old_file_ref(cmd->io_fds, false))
		return;
	cmd->io_fds->outfile = ft_strdup(cmd->argv[1]);
	if (cmd->io_fds->outfile && cmd->io_fds->outfile[0] == '\0')
	{
		ft_error(3, data);
		return;
	}
	cmd->io_fds->out_fd = open(cmd->io_fds->outfile, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (cmd->io_fds->out_fd == -1)
	{
		perror("file");
		return;
	}

	t_cmd *current = cmd->prev;
	while (current && current->type == REDIR_OUT)
	{
		current->io_fds->outfile = cmd->io_fds->outfile;
		current->io_fds->out_fd = cmd->io_fds->out_fd;
		current = current->prev;
	}
	if (current && current->type != REDIR_OUT)
	{
		current->io_fds->outfile = cmd->io_fds->outfile;
		current->io_fds->out_fd = cmd->io_fds->out_fd;
	}
}

void init_append(t_cmd *cmd, t_data *data)
{
	init_io(&cmd->io_fds);
	if (!remove_old_file_ref(cmd->io_fds, false))
		return;
		
	cmd->io_fds->outfile = ft_strdup(cmd->argv[1]);
	if (cmd->io_fds->outfile && cmd->io_fds->outfile[0] == '\0')
	{
		ft_error(3, data);
		return;
	}
	cmd->io_fds->out_fd = open(cmd->io_fds->outfile, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (cmd->io_fds->out_fd == -1)
	{
		perror("open");
		return;
	}

	t_cmd *current = cmd->prev;
	while (current && current->type == REDIR_OUT)
	{
		current->io_fds->outfile = cmd->io_fds->outfile;
		current->io_fds->out_fd = cmd->io_fds->out_fd;
		current = current->prev;
	}
	if (current && current->type != REDIR_OUT)
	{
		current->io_fds->outfile = cmd->io_fds->outfile;
		current->io_fds->out_fd = cmd->io_fds->out_fd;
	}
}

void init_read_from(t_cmd *cmd, t_data *data)
{
	init_io(&cmd->io_fds);
	if (!remove_old_file_ref(cmd->io_fds, true))
		return;
		
	cmd->io_fds->infile = ft_strdup(cmd->argv[1]);
	cmd->io_fds->in_fd = open(cmd->io_fds->infile, O_RDONLY);
	if (cmd->io_fds->in_fd == -1)
	{
		ft_putstr_fd("infile error\n", 2);
		return;
	}

	t_cmd *current = cmd->prev;
	while (current && current->type == REDIR_IN)
	{
		current->io_fds->infile = cmd->io_fds->infile;
		current->io_fds->in_fd = cmd->io_fds->in_fd;
		current = current->prev;
	}
	if (current && current->type != REDIR_IN)
	{
		current->io_fds->infile = cmd->io_fds->infile;
		current->io_fds->in_fd = cmd->io_fds->in_fd;
	}
	current = cmd->next;
	while (current)
	{
		init_io(&current->io_fds);
		if (current->type != REDIR_IN)
		{
			current->io_fds->infile = cmd->io_fds->infile;
			current->io_fds->in_fd = cmd->io_fds->in_fd;
			break;
		}
		else
			break ;
		current = current->next;
	}
}