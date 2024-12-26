/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 18:26:39 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

int	init_command(t_cmd *cmd, t_data *data)
{
	(void)data;
	if (should_pipe(cmd) || (cmd->next && cmd->next->type == CMD))
		cmd->pipe_output = true;
	return (1);
}

int	fork_and_exec(t_data *data)
{
	data->pid = fork();
	if (data->pid != -1)
		signal(SIGINT, handlehang);
	if (data->pid == -1)
		return (ft_putstr_fd("fork error\n", 2), 0);
	if (data->pid == 0)
		g_exit_status = exec_cmd(data->cmd->argv, data->envp_arr, data);
	waitpid(data->pid, &g_exit_status, 0);
	handle_child_term(g_exit_status);
	return (1);
}

void	lstadd_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_io_fds	*dup_io(t_io_fds *io)
{
	t_io_fds	*new;

	new = (t_io_fds *)malloc(sizeof(t_io_fds));
	if (!new)
		return (NULL);
	new->in_fd = io->in_fd;
	new->out_fd = io->out_fd;
	new->heredoc_name = ft_strdup(io->heredoc_name);
	new->infile = ft_strdup(io->infile);
	new->outfile = ft_strdup(io->outfile);
	return (new);
}
