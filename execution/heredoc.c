/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 16:12:57 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*random_file_name(void)
{
	char		*name;
	int			i;
	int			fd;
	int			k;
	const char	charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";

	name = malloc(30);
	ft_strlcpy(name, "/tmp/", 6);
	k = 0;
	i = 0;
	while (k < 15)
	{
		fd = open("/dev/urandom", O_RDONLY);
		read(fd, &i, 1);
		name[k + 5] = charset[i % 36];
		ft_close(fd);
		k++;
	}
	name[k + 5] = '\0';
	return (name);
}

void	write_to_file(t_data *data, t_cmd *cmd, int fd, char *file)
{
	char	*line;

	line = NULL;
	while (g_exit_status == -1)
	{
		line = readline("> ");
		if (!line)
			execute_signal(data, fd, file);
		if (ft_strncmp(line, cmd->argv[1], 0) == 0)
		{
			free(line);
			break ;
		}
		if (ft_strchr(line, '$'))
			line = expand_string(line, data->envp);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void	set_ios(t_cmd *cmd, int temp_fd)
{
	t_cmd	*current;

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

void	end_and_reset(t_data *data, char *temp_file, int temp_fd)
{
	free(temp_file);
	ft_close(temp_fd);
	reset_shell(data, 0);
}

int	init_heredoc(t_cmd *cmd, t_data *data)
{
	char	*temp_file;
	int		temp_fd;
	int		fork_id;

	g_exit_status = -1;
	temp_file = random_file_name();
	fork_id = fork();
	if (fork_id != 0)
		signal(SIGINT, handlehang);
	if (fork_id == 0)
	{
		signal(SIGINT, handledoc);
		temp_fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (temp_fd == -1)
			return (perror("open"), 0);
		write_to_file(data, cmd, temp_fd, temp_file);
		end_and_reset(data, temp_file, temp_fd);
	}
	waitpid(0, &g_exit_status, 0);
	handle_child_term(g_exit_status);
	temp_fd = open(temp_file, O_RDONLY, 0644);
	if (temp_fd == -1)
		return (free(temp_file), 0);
	set_ios(cmd, temp_fd);
	return (close(temp_fd), unlink(temp_file), free(temp_file), 1);
}
