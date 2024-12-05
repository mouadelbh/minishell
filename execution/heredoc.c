/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/05 11:03:08 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_heredoc(t_cmd *cmd, t_data *data)
{
	char	*temp;

	int temp_fd = open("/tmp/ncajha3f83", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
		if (ft_strchr(line, '$'))
		{
			line[0] = -1;
			line = find_and_replace(line, data->envp, 0);
		}
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
	temp_fd = open("/tmp/ncajha3f83", O_RDONLY, 0644);
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
