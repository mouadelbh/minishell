/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/03 01:45:05 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int should_pipe(t_cmd *cmd)
{
	t_cmd	*temp = cmd;
	int		pipe_count = 0;

	while (temp)
	{
		if (temp->type == CMD)
			pipe_count++;
		temp = temp->next;
	}
	if (pipe_count > 1)
		return (1);
	return (0);
}

void	init_command(t_cmd *cmd, t_data *data)
{
	if (should_pipe(cmd) || (cmd->next && cmd->next->type == CMD))
		cmd->pipe_output = true;
}

int	valid_command(t_cmd *cmd, t_data *data)
{
	char	*full_command;

	full_command = get_full_cmd(cmd->argv[0], data->envp_arr);
	if (access(full_command, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		// free(full_command);
		data->status = 1;
		return (0);
	}
	// free(full_command);
	return (1);
}

int	check_cmd(char *cmd, t_data *data)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
		return (0);
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		data->status = 126;
		return (1);
	}
	return (0);
}

int	check_permission(char *path, t_data *data)
{
	if (access(path, X_OK) != 0 && access(path, F_OK) == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(data->cmd->argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		// free(path);
		data->status = 126;
		return (1);
	}
	return (0);
}

int	command_is_valid(t_data *data, t_cmd *cmd, int is_builtin)
{
	char	*full_cmd;
	if (is_builtin)
	{
		return (1);
	}
	full_cmd = get_full_cmd(cmd->argv[0], data->envp_arr);
	if (check_cmd(cmd->argv[0], data) || check_permission(full_cmd, data))
		return (free(full_cmd), 0);
	// free(full_cmd);
	if (cmd->type == CMD)
		return (valid_command(cmd, data));
	return (0);
}

int	handle_execute(t_data *data)
{
	t_cmd	*cmd;
	t_cmd	*temp;

	cmd = data->cmd;
	temp = cmd;
	while (data->pid != 0 && cmd)
	{
		if (command_is_valid(data, cmd, builtin(cmd->argv[0])))
		{
			data->status = 0;
			data->pid = fork();
			if (data->pid == -1)
				return (ft_putstr_fd("fork error\n", 2), 1);
		}
		else
			data->status = 1;
		if (data->pid == 0)
			data->status = execute_command(data, cmd);
		// waitpid(data->pid, &data->status, 0);
		cmd = cmd->next;
	}
	return (close_file(data), data->status);
}

int exec_cmd(char **command, char **envp, t_data *data)
{
	char	*path;

	path = NULL;
	if (command[0][0] == '/')
		path = ft_strdup(command[0]);
	else if (command[0][0] != '\0')
		path = get_full_cmd(command[0], envp);
	if (!path)
		return (ft_error(7, data), 1);
	if (execve(path, command, envp) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		data->status = 1;
		free(path);
		return (1);
	}
	return (0);
}

int single_command(t_data *data, char *cmd)
{
	t_line *temp = data->head;
	char	*path;

	while (temp)
	{
		if (temp->next && temp->next->type == 7)
			temp = temp->next;
		path = get_full_cmd(data->cmd->argv[0], data->envp_arr);
		if (builtin(data->cmd->argv[0]))
			data->status = exec_builtin(data, data->cmd->argv);
		else
		{
			if (check_cmd(path, data) == 1 || check_permission(path, data) == 1)
			{
				free(path);
				printf("No such file or directory\n");
				global.g_exit_status = 126;
				return (126);
			}
			data->pid = fork();
			if (data->pid == -1)
				return (ft_error(1, data));
			if (data->pid == 0)
				data->status = exec_cmd(data->cmd->argv, data->envp_arr, data);
			waitpid(data->pid, &data->status, 0);
			if (data->pid == 0)
				exit(data->status);
		}
		free(path);
		temp = temp->next;
	}
	return (data->status);
}

t_cmd	*init_new_cmd(t_cmd *src)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = src->argv;
	new->cmd = src->cmd;
	new->type = src->type;
	new->pipe_fd = src->pipe_fd;
	new->pipe_output = src->pipe_output;
	new->io_fds = src->io_fds;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	free_cmd_node(t_cmd *cmd)
{
	if (cmd->argv)
		free_arr(cmd->argv);
	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	if (cmd->io_fds)
	{
		if (cmd->io_fds->infile)
			free(cmd->io_fds->infile);
		if (cmd->io_fds->outfile)
			free(cmd->io_fds->outfile);
		if (cmd->io_fds->heredoc_name)
			free(cmd->io_fds->heredoc_name);
		free(cmd->io_fds);
	}
	free(cmd);
	cmd = NULL;
}

t_cmd	*set_command_list(t_cmd *cmd)
{
	t_cmd	*new_list;
	t_cmd	*current;
	t_cmd	*temp;

	if (!cmd)
		return (NULL);
	while (cmd && cmd->type != CMD && cmd->next)
	{
		free_cmd_node(cmd);
		cmd = cmd->next;
	}
	if (!cmd)
		return (NULL);
	new_list = init_new_cmd(cmd);
	if (!new_list)
		return (NULL);
	current = new_list;
	cmd = cmd->next;
	while (cmd)
	{
		if (cmd->type == CMD)
		{
			temp = init_new_cmd(cmd);
			if (!temp)
			{
				while (new_list)
				{
					temp = new_list->next;
					free(new_list);
					new_list = temp;
				}
				return (NULL);
			}
			current->next = temp;
			temp->prev = current;
			current = temp;
		}
		cmd = cmd->next;
	}
	return (new_list);
}

int	complex_command(t_data *data)
{
	t_line	*temp = data->head;
	int		ret;

	if (data->cmd)
	{
		if (!create_files(data->cmd, data))
		{
			// free_cmd_list(&data->cmd);
			return (1);
		}
		data->cmd = set_command_list(data->cmd);
		ret = set_values(data);
		return (handle_execute(data));
	}
	else
		ft_putstr_fd("No command found\n", 2);
	return (0);
}
