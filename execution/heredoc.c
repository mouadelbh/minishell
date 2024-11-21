/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/11/20 20:16:19 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_heredoc(t_cmd *cmd, t_data *data)
{
	// init_io(&cmd->io_fds);
	int temp_fd = open("/tmp/heredoc_zizi", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		perror("heredoc temp file");
		return;
	}
	char *line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (strcmp(line, cmd->argv[1]) == 0)
		{
			free(line);
			break;
		}
		write(temp_fd, line, strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	close(temp_fd);
	temp_fd = open("/tmp/heredoc_zizi", O_RDONLY, 0644);
	cmd->io_fds->in_fd = temp_fd;
	t_cmd *current = cmd->prev;
	while (current && current->type != CMD)
	{
		current->io_fds->in_fd = temp_fd;
		current = current->prev;
	}
	if (current && current->type == CMD)
		current->io_fds->in_fd = temp_fd;
}