/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:28:33 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/05 13:36:49 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	show_command_info(t_cmd *command)
{
	t_cmd *cmd = command;
	while (cmd)
	{
		ft_putstr_fd("------------------\n", 2);
		ft_putstr_fd("Command: ", 2);
		ft_putstr_fd(cmd->cmd?cmd->cmd:"NULL", 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The fd in is: ", 2);
		ft_putnbr_fd(cmd->io_fds->in_fd, 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The file to read from: ", 2);
		ft_putstr_fd(cmd->io_fds->infile?cmd->io_fds->infile:"NULL", 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The file to write to: ", 2);
		ft_putstr_fd(cmd->io_fds->outfile?cmd->io_fds->outfile:"NULL", 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The fd out is: ", 2);
		ft_putnbr_fd(cmd->io_fds->out_fd, 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("Type: ", 2);
		ft_putnbr_fd(cmd->type, 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("Is piped? ", 2);
		ft_putstr_fd(cmd->pipe_output?"Yes":"No", 2);
		ft_putchar_fd('\n', 2);
		cmd = cmd->next;
	}
}

void	show_command_ios(t_cmd *cmd)
{
	t_cmd	*temp = cmd;
	while (temp)
	{
		show_io_fds(temp->io_fds);
		temp = temp->next;
	}
}

void	show_io_fds(t_io_fds *io_fds)
{
	if (!io_fds)
	{
		ft_putstr_fd("io_fds is NULL\n", 2);
		return;
	}
	ft_putstr_fd("in_fd: ", 2);
	ft_putnbr_fd(io_fds->in_fd, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("out_fd: ", 2);
	ft_putnbr_fd(io_fds->out_fd, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("infile: ", 2);
	ft_putstr_fd(io_fds->infile?io_fds->infile:"Null infile", 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("outfile: ", 2);
	ft_putstr_fd(io_fds->outfile?io_fds->outfile:"Null outfile", 2);
	ft_putchar_fd('\n', 2);
}

void	printa(char *message, char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	if (!*str)
		return ;
	printf("%s: ", message);
	while (str[i])
	{
		printf("%s\n", str[i]);
		i++;
	}
}

void	debug()
{
	static int count;

	if (!count)
		count = 0;
	// printf("Here: %d\n", count);
	ft_putstr_fd("Debugging\n", 2);
	ft_putnbr_fd(count, 2);
	ft_putchar_fd('\n', 2);
	count++;
}

static void	print_cmd_args(t_cmd *cmd)
{
	int	i;

	if (!cmd->argv)
		return ;
	i = 0;
	while (cmd->argv[i])
	{
		printf("\tArgs[%d] = %s\n", i, cmd->argv[i]);
		i++;
	}
}

static void	print_cmd_io(t_cmd *cmd)
{
	if (!cmd->io_fds)
		return ;
	if (cmd->io_fds->infile)
	{
		printf("\tInfile: %s\n", cmd->io_fds->infile);
		printf("\t\tfd_in: %d\n", cmd->io_fds->in_fd);
	}
	if (cmd->io_fds->heredoc_name)
		printf("\tHeredoc delimiter: %s\n", cmd->io_fds->heredoc_name);
	if (cmd->io_fds->outfile)
	{
		printf("\tOutfile: %s\n", cmd->io_fds->outfile);
		printf("\t\tfd_in: %d\n", cmd->io_fds->out_fd);
	}
}

static void	print_cmd_list(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmd;
	printf("\n---- COMMAND LIST\n");
	while (cmd)
	{
		printf("--- Command = %s\n", cmd->cmd);
		print_cmd_args(cmd);
		printf("\tPipe_output = %d\n", cmd->pipe_output);
		print_cmd_io(cmd);
		if (cmd->prev == NULL)
			printf("\tprev = NULL\n");
		else
			printf("\tprev = %s\n", cmd->prev->cmd);
		if (cmd->next == NULL)
			printf("\tnext = NULL\n");
		else
			printf("\tnext = %s\n", cmd->next->cmd);
		printf("\n");
		cmd = cmd->next;
	}
	printf("\n");
}

void print_open_fds(t_cmd *cmd)
{
	t_cmd	*temp = cmd;

	while (temp)
	{
		if (temp->io_fds)
		{
			if (temp->io_fds->in_fd != -1)
				printf("Infile: %s\n", temp->io_fds->infile);
			if (temp->io_fds->out_fd != -1)
				printf("Outfile: %s\n", temp->io_fds->outfile);
		}
		temp = temp->next;
	}
}
