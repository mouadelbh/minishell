/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/16 23:12:29 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handledoc(int sig)
{
	(void)sig;
	exit_status = CTRL_C;
	close(0);
}

char	*random_file_name(void)
{
	char	*name;
	int		i;
	int		fd;
	int		k;
	const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";

	name = malloc(21);
	ft_strlcpy(name, "/tmp/", 6);
	k = 0;
	while (k < 15)
	{
		fd = open("/dev/urandom", O_RDONLY);
		read(fd, &i, 1);
		name[k + 5] = charset[i % 36];
		close(fd);
		k++;
	}
	name[k + 5] = '\0';
	return (name);
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

int init_heredoc(t_cmd *cmd, t_data *data)
{
	char	*temp;
	char	*line;
	char	*temp_file;
	int		temp_fd;
	int		fork_id;
	int		tmp;
	t_cmd	*current;

	line = NULL;
	tmp = exit_status;
	exit_status = -1;
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
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				if (exit_status == -1)
					perror("minishell: warning: here-document delimited by end-of-file\n");
				unlink(temp_file);
				free(temp_file);
				close(temp_fd);
				free_all(data, 1);
				exit(0);
			}
			if (ft_strchr(line, '$'))
			{
				line[0] = -1;
				line = find_and_replace(line, data->envp, 0);
			}
			if (ft_strncmp(line, cmd->argv[1], 0) == 0)
			{
				free(line);
				break;
			}
			write(temp_fd, line, ft_strlen(line));
			write(temp_fd, "\n", 1);
			free(line);
		}
		close(temp_fd);
		exit(0);
	}
	exit_status = tmp;
	waitpid(0, &exit_status, 0);
	temp_fd = open(temp_file, O_RDONLY, 0644);
	if (temp_fd == -1)
	{
		perror("open");
		return (0);
	}
	cmd->io_fds->in_fd = temp_fd;
	current = cmd->prev;
	while (current && current->type != CMD)
	{
		current->io_fds->in_fd = temp_fd;
		current = current->prev;
	}
	if (current && current->type == CMD)
		current->io_fds->in_fd = temp_fd;
	unlink(temp_file);
	free(temp_file);
	return (1);
}
