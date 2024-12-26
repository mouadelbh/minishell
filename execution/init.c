/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:24:39 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 15:26:21 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	default_io(t_io_fds *io)
{
	io->in_fd = STDIN_FILENO;
	io->out_fd = STDOUT_FILENO;
	io->infile = NULL;
	io->outfile = NULL;
	io->heredoc_name = NULL;
	io->stdin_backup = -1;
	io->stdout_backup = -1;
}

void	file_error(t_cmd *cmd, char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->argv[1], 2);
	ft_putstr_fd(" ", 2);
	ft_putstr_fd(str, 2);
}

void	init_cmd(t_cmd *cmd)
{
	cmd->argv = NULL;
	cmd->cmd = NULL;
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
		perror("malloc");
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

int	init_append(t_cmd *cmd)
{
	t_cmd	*current;

	if (!remove_old_file_ref(cmd->io_fds, false) || cmd->file_error == 0)
		return (0);
	cmd->io_fds->outfile = ft_strdup(cmd->argv[1]);
	if (cmd->io_fds->outfile[0] == '\0' \
	|| (cmd->io_fds->outfile[0] == 36 \
	&& cmd->io_fds->outfile[1] != '\0'))
		return (file_error(cmd, "ambigious redirect\n"), 0);
	if (cmd->io_fds->outfile && cmd->io_fds->outfile[0] == '\0')
		return (file_error(cmd, "No such file or directory\n"), 0);
	if (cmd->prev && cmd->prev->file_error != 1)
		return (0);
	cmd->io_fds->out_fd = open(cmd->io_fds->outfile, O_RDWR | O_APPEND \
	| O_CREAT, 0644);
	if (cmd->io_fds->out_fd == -1)
		return (perror(cmd->argv[1]), 0);
	current = cmd->prev;
	while (current && current->type != CMD)
	{
		free(current->io_fds->outfile);
		current->io_fds->outfile = ft_strdup(cmd->io_fds->outfile);
		current->io_fds->out_fd = cmd->io_fds->out_fd;
		current = current->prev;
	}
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

	if (!remove_old_file_ref(cmd->io_fds, false) || cmd->file_error == 0)
		return (0);
	if (cmd->io_fds->outfile)
		free(cmd->io_fds->outfile);
	cmd->io_fds->outfile = ft_strdup(cmd->argv[1]);
	if (cmd->io_fds->outfile[0] == '\0' \
	|| (cmd->io_fds->outfile[0] == 36 \
	&& cmd->io_fds->outfile[1] != '\0'))
		return (file_error(cmd, "ambigious redirect\n"), 0);
	if (cmd->io_fds->outfile && cmd->io_fds->outfile[0] == '\0')
		return (file_error(cmd, "No such file or directory\n"), 0);
	if (cmd->prev && cmd->prev->file_error != 1)
		return (0);
	cmd->io_fds->out_fd = open(cmd->io_fds->outfile, O_RDWR | O_TRUNC \
	| O_CREAT, 0644);
	if (cmd->io_fds->out_fd == -1)
		return (perror(cmd->argv[1]), 0);
	current = cmd->prev;
	if (cmd->prev && cmd->prev->type == PIPE)
		return (0);
	while (current && current->type != CMD)
	{
		free(current->io_fds->outfile);
		current->io_fds->outfile = ft_strdup(cmd->io_fds->outfile);
		current->io_fds->out_fd = cmd->io_fds->out_fd;
		current = current->prev;
	}
	if (current && current->type != REDIR_OUT)
	{
		free(current->io_fds->outfile);
		current->io_fds->outfile = ft_strdup(cmd->io_fds->outfile);
		current->io_fds->out_fd = cmd->io_fds->out_fd;
	}
	return (1);
}

int	init_read_from(t_cmd *cmd)
{
	t_cmd	*current;

	cmd->io_fds->infile = ft_strdup(cmd->argv[1]);
	if ((cmd->prev && cmd->prev->file_error != 1) || cmd->file_error == 0)
		return (0);
	cmd->io_fds->in_fd = open(cmd->io_fds->infile, O_RDONLY);
	if (access(cmd->io_fds->infile, F_OK) == 0 \
	&& access(cmd->io_fds->infile, R_OK) == -1)
		return (file_error(cmd, ": Permission denied\n"), 0);
	if (access(cmd->io_fds->infile, F_OK) == -1)
		return (file_error(cmd, ": No such file or directory\n"), 0);
	// if (!remove_old_file_ref(cmd->io_fds, true))
	// 	return (0);
	if (cmd->io_fds->in_fd == -1)
		return (perror(cmd->argv[1]), 0);
	current = cmd->prev;
	while (current && current->type != CMD)
	{
		free(current->io_fds->infile);
		current->io_fds->infile = ft_strdup(cmd->io_fds->infile);
		current->io_fds->in_fd = cmd->io_fds->in_fd;
		current = current->prev;
	}
	if (current && current->type != REDIR_IN)
	{
		free(current->io_fds->infile);
		current->io_fds->infile = ft_strdup(cmd->io_fds->infile);
		current->io_fds->in_fd = cmd->io_fds->in_fd;
	}
	return (1);
}
