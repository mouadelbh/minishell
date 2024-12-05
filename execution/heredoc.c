/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/05 12:55:12 by zelbassa         ###   ########.fr       */
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

int is_file_closed(int fd)
{
	struct stat	st;
    // Use fstat to check if the file descriptor is valid
    if (fstat(fd, &st) == -1) {
        // If fstat fails with EBADF, the file descriptor is likely closed
        if (errno == EBADF) {
            return 1;  // File is closed
        }
        // Other errors might occur
        perror("File descriptor check error");
        return 0;
    }
    
    // File descriptor is still open
    return 0;
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
	if (!is_file_closed(temp_fd))
	{
		// perror("stat");
		return ;
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
