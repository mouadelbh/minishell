/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/05 12:02:12 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	skibidi(int sig)
{
	(void)sig;
	// int fd = dup(0);
	// close(fd);
	close(0);
	write(1, "\n", 1);
}

void init_heredoc(t_cmd *cmd, t_data *data)
{
	char	*line;
	int		temp_fd;
	t_cmd	*current;
	int		id;
	struct stat	st;

	temp_fd = open("/tmp/ncajha3f83", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		perror("heredoc temp file");
		return;
	}
	line = NULL;
	id = fork();
	if (id == 0)
	{		
		while (1)
		{
			signal(SIGINT, skibidi);
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
		exit(0);
	}
	waitpid(id, &data->status, 0);
	if (stat("/tmp/ncajha3f83", &st) == -1)
	{
		// perror("stat");
		return;
	}
	close(temp_fd);
	temp_fd = open("/tmp/ncajha3f83", O_RDONLY, 0644);
	cmd->io_fds->in_fd = temp_fd;
	current = cmd->prev;
	while (current && current->type != CMD)
	{
		current->io_fds->in_fd = temp_fd;
		current = current->prev;
	}
	if (current && current->type == CMD)
		current->io_fds->in_fd = temp_fd;
}
