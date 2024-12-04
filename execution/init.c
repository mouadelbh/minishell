/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:24:39 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/04 11:16:21 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_cmd(t_cmd *cmd)
{
	cmd->argv = NULL;
	cmd->cmd = NULL;
	// cmd->pipe_fd = NULL;
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

int init_append(t_cmd *cmd, t_data *data)
{
	if (!remove_old_file_ref(cmd->io_fds, false))
		return (0);
	cmd->io_fds->outfile = ft_strdup(cmd->argv[1]);
	if (cmd->io_fds->outfile && cmd->io_fds->outfile[0] == '\0')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd("No such file or directory\n", 2);
		return (0);
	}
	if (cmd->io_fds->outfile[0] == '\0' || (cmd->io_fds->outfile[0] == 36 && cmd->io_fds->outfile[1] != '\0'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd("ambigious redirect\n", 2);
		return (0);
	}
	cmd->io_fds->out_fd = open(cmd->io_fds->outfile, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (cmd->io_fds->out_fd == -1)
		return (perror("open"), 0);
	t_cmd *current = cmd->prev;
	while (current && current->type != CMD)
	{
		current->io_fds->outfile = cmd->io_fds->outfile;
		current->io_fds->out_fd = cmd->io_fds->out_fd;
		current = current->prev;
	}
	if (current && current->type != APPEND)
	{
		current->io_fds->outfile = cmd->io_fds->outfile;
		current->io_fds->out_fd = cmd->io_fds->out_fd;
	}
	return (1);
}

int init_write_to(t_cmd *cmd, t_data *data)
{
	if (!remove_old_file_ref(cmd->io_fds, false))
		return (0);
	cmd->io_fds->outfile = ft_strdup(cmd->argv[1]);
	if (cmd->io_fds->outfile && cmd->io_fds->outfile[0] == '\0')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd("No such file or directory\n", 2);
		return (0);
	}
	if (cmd->io_fds->outfile[0] == '\0' || (cmd->io_fds->outfile[0] == 36 && cmd->io_fds->outfile[1] != '\0'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd("ambigious redirect\n", 2);
		return (0);
	}
	cmd->io_fds->out_fd = open(cmd->io_fds->outfile, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (cmd->io_fds->out_fd == -1)
		return (perror("open"), 0);
	t_cmd *current = cmd->prev;
	while (current && current->type != CMD)
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
	return (1);
}

int	init_read_from(t_cmd *cmd, t_data *data)
{
	if (!remove_old_file_ref(cmd->io_fds, true))
		return (0);
	cmd->io_fds->infile = ft_strdup(cmd->argv[1]);
	if (access(cmd->io_fds->infile, F_OK) == 0 && access(cmd->io_fds->infile, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (0);
	}
	if (access(cmd->io_fds->infile, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	cmd->io_fds->in_fd = open(cmd->io_fds->infile, O_RDONLY);
	if (cmd->io_fds->in_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	t_cmd *current = cmd->prev;
	while (current && current->type != CMD)
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
	return (1);
}
