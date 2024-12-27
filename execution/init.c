/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:24:39 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/27 14:07:28 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_append(t_cmd *cmd)
{
	t_cmd	*current;

	if (!check_file_refs(cmd))
		return (0);
	current = cmd->prev;
	if (cmd->prev && cmd->prev->type == PIPE)
		return (0);
	while (current && current->type != CMD)
		current = current->prev;
	if (current && current->type != APPEND)
	{
		free(current->io_fds->outfile);
		current->io_fds->outfile = ft_strdup(cmd->io_fds->outfile);
		current->io_fds->out_fd = cmd->io_fds->out_fd;
	}
	return (1);
}

int	init_write_to(t_cmd *cmd)
{
	t_cmd	*current;

	if (!check_file_refs(cmd))
		return (0);
	current = cmd->prev;
	if (cmd->prev && cmd->prev->type == PIPE)
		return (0);
	while (current && current->type != CMD)
		current = current->prev;
	if (current && current->type != REDIR_OUT)
	{
		free(current->io_fds->outfile);
		current->io_fds->outfile = ft_strdup(cmd->io_fds->outfile);
		current->io_fds->out_fd = cmd->io_fds->out_fd;
	}
	return (1);
}

int	check_read_file_ref(t_cmd *cmd)
{
	cmd->io_fds->infile = ft_strdup(cmd->argv[1]);
	if ((cmd->prev && cmd->prev->file_error != 1) || cmd->file_error == 0)
		return (0);
	cmd->io_fds->in_fd = open(cmd->io_fds->infile, O_RDONLY);
	if (access(cmd->io_fds->infile, F_OK) == 0 \
	&& access(cmd->io_fds->infile, R_OK) == -1)
		return (file_error(cmd, ": Permission denied\n"), 0);
	if (access(cmd->io_fds->infile, F_OK) == -1)
		return (file_error(cmd, ": No such file or directory\n"), 0);
	if (cmd->io_fds->in_fd == -1)
		return (perror(cmd->argv[1]), 0);
	return (1);
}

int	init_read_from(t_cmd *cmd)
{
	t_cmd	*current;

	if (!check_read_file_ref(cmd))
		return (0);
	current = cmd->prev;
	while (current && current->type != CMD)
		current = current->prev;
	if (current && current->type != REDIR_IN)
	{
		free(current->io_fds->infile);
		current->io_fds->infile = ft_strdup(cmd->io_fds->infile);
		current->io_fds->in_fd = cmd->io_fds->in_fd;
	}
	return (1);
}
