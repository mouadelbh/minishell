/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/08 21:38:20 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handledoc(int sig)
{
	(void)sig;
	exit_status = CTRL_C;
	close(0);
	exit(0);
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

// void init_heredoc(t_cmd *cmd, t_data *data)
// {
// 	char	*line;
// 	int		temp_fd;
// 	t_cmd	*current;

// 	temp_fd = open("/tmp/ncajha3f83", O_CREAT | O_RDWR | O_TRUNC, 0644);
// 	if (temp_fd == -1)
// 	{
// 		perror("heredoc temp file");
// 		return;
// 	}
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			perror("minishell: warning: \
// 			here-document delimited by end-of-file\n");
// 			free_all(data, 1);
// 			exit(0);
// 			break;
// 		}
// 		if (strcmp(line, cmd->argv[1]) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		if (ft_strchr(line, '$'))
// 		{
// 			line[0] = -1;
// 			line = expand_string(line, data->envp);
// 		}
// 		write(temp_fd, line, strlen(line));
// 		write(temp_fd, "\n", 1);
// 		free(line);
// 	}
// 	close(temp_fd);
// 	temp_fd = open("/tmp/ncajha3f83", O_RDONLY, 0644);
// 	cmd->io_fds->in_fd = temp_fd;
// 	current = cmd->prev;
// 	while (current && current->type != CMD)
// 	{
// 		current->io_fds->in_fd = temp_fd;
// 		current = current->prev;
// 	}
// 	if (current && current->type == CMD)
// 		current->io_fds->in_fd = temp_fd;
// }

int init_heredoc(t_cmd *cmd, t_data *data)
{
	char	*temp;
	char	*line;
	int		temp_fd;
	int		fork_id;
	t_cmd	*current;

	temp_fd = open("/tmp/jc03fjkdc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		perror("heredoc temp file");
		return (0);
	}
	line = NULL;
	fork_id = fork();
	if (data->pid != -1)
		change_signal();
	if (fork_id == 0)
	{
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				perror("minishell: warning: \
				here-document delimited by end-of-file\n");
				free_all(data, 1);
				exit(0);
				break;
			}
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
	waitpid(0, &data->status, 0);
	temp_fd = open("/tmp/jc03fjkdc", O_RDONLY, 0644);
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