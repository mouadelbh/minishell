/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 17:50:28 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 17:52:48 by zelbassa         ###   ########.fr       */
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

int	check_file_refs(t_cmd *cmd)
{
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
	return (1);
}