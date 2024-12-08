/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/09 00:01:57 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include <sys/wait.h>

int	init_command(t_cmd *cmd, t_data *data)
{
	if (should_pipe(cmd) || (cmd->next && cmd->next->type == CMD))
		cmd->pipe_output = true;
	return (1);
}

int	handle_execute(t_data *data)
{
	t_cmd	*cmd;
	t_cmd	*temp;

	cmd = data->cmd;
	temp = cmd;
	while (cmd)
	{
		if (command_is_valid(data, cmd, builtin(cmd->argv[0])))
		{
			data->status = 0;
			data->pid = fork();
			if (data->pid != -1)
				change_signal();
			if (data->pid == -1)
				return (ft_putstr_fd("fork error\n", 2), 1);
		}
		else
			data->status = 1;
		if (data->pid == 0)
			data->status = execute_command(data, cmd);
		cmd = cmd->next;
	}
	waitpid(0, &data->status, 0);
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
		if (builtin(data->cmd->argv[0]))
			return (exec_builtin(data, data->cmd->argv));
		else
		{
			path = get_full_cmd(data->cmd->argv[0], data->envp_arr);
			if (check_cmd(path, data) == 1 || check_permission(path, data) == 1)
			{
				free(path);
				exit_status = 126;
				return (126);
			}
			data->pid = fork();
			if (data->pid != -1)
				change_signal();
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
