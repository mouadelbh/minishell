/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/15 22:44:46 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handledoc(int sig)
{
	(void)sig;
	exit_status = CTRL_C;
	close(0);
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
	int		temp_fd;
	int		fork_id;
	int		tmp;
	t_cmd	*current;

	line = NULL;
	tmp = exit_status;
	exit_status = -1;
	fork_id = fork();
	if (fork_id != 0)
		signal(SIGINT, handlehang);
	if (fork_id == 0)
	{
		signal(SIGINT, handledoc);
		unlink("/tmp/jc03fjkdc");
		temp_fd = open("/tmp/jc03fjkdc", O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (temp_fd == -1)
			return (perror("open"), 0);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				if (exit_status == -1)
					perror("minishell: warning: here-document delimited by end-of-file\n");
				unlink("/tmp/jc03fjkdc");
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
	temp_fd = open("/tmp/jc03fjkdc", O_RDONLY, 0644);
	if (temp_fd == -1)
		return (0);
	cmd->io_fds->in_fd = temp_fd;
	current = cmd->prev;
	while (current && current->type != CMD)
	{
		current->io_fds->in_fd = temp_fd;
		current = current->prev;
	}
	if (current && current->type == CMD)
		current->io_fds->in_fd = temp_fd;
	return (1);
}
