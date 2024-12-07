/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/05 15:35:51 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../includes/minishell.h"

void	handledoc(int sig)
{
	(void)sig;
	exit_status = 127;
	close(0);
}

int is_file_closed(int fd)
{
	struct stat	st;
	if (fstat(fd, &st) == -1)
	{
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

char	*expand_string(char *line, t_list *envp)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && line[i + 1] != '$')
			line[i] = -1;
		i++;
	}
	line = find_and_replace(line, envp, 0);
	return (line);
}

void	print_warning(int i, char *str)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(i, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("')\n", 2);
}

void init_heredoc(t_cmd *cmd, t_data *data)
{
	char	*line;
	int		temp_fd;
	t_cmd	*current;
	int		id;
	int		tmp;
	int		i;
	struct stat	st;

	temp_fd = open("/tmp/ncajha3f83", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		perror("heredoc temp file");
		return;
	}
	tmp = exit_status;
	exit_status = 0;
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, handledoc);
		i = 0;
		while (1)
		{
			line = readline("> ");
			i++;
			if (!line)
			{
				if (exit_status == 0)
					print_warning(i, cmd->argv[1]);
				break;
			}
			if (strcmp(line, cmd->argv[1]) == 0)
			{
				free(line);
				break;
			}
			if (ft_strchr(line, '$'))
			{
				line[0] = -1;
				line = expand_string(line, data->envp);
			}
			write(temp_fd, line, strlen(line));
			write(temp_fd, "\n", 1);
			free(line);
		}
		close(temp_fd);
		exit(0);
	}
	exit_status = tmp;
	waitpid(id, &data->status, 0);
	// if (!is_file_closed(temp_fd))
	// {
	// 	// perror("stat");
	// 	return ;
	// }
	// close(temp_fd);
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
