/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/15 22:36:00 by zelbassa         ###   ########.fr       */
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
	char	*full_cmd;

	cmd = data->cmd;
	temp = cmd;
	while (cmd)
	{
		if (cmd->type == CMD)
			data->pid = fork();
		if (data->pid != -1)
			signal(SIGINT, handlehang);
		if (data->pid == -1)
			return (ft_putstr_fd("fork error\n", 2), 1);
		if (data->pid == 0)
			execute_command(data, cmd);
		cmd = cmd->next;
	}
	exit_status = WEXITSTATUS(close_file(data, temp));
	return (exit_status);
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
		ft_error(7, data);
	if (execve(path, command, envp) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit_status = 127;
		free(path);
		exit(127);
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
			exit_status = exec_builtin(data, data->cmd->argv);
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
				signal(SIGINT, handlehang);
			if (data->pid == -1)
				return (ft_error(1, data));
			if (data->pid == 0)
				exit_status = exec_cmd(data->cmd->argv, data->envp_arr, data);
			waitpid(data->pid, &exit_status, 0);
			exit_status = WEXITSTATUS(exit_status);
		}
		temp = temp->next;
	}
	update_env(data->cmd, data);
	return (exit_status);
}

int	complex_command(t_data *data)
{
	t_line	*temp = data->head;
	int		ret;
	t_cmd	*cmd;

	if (data->cmd)
	{
		if (!create_files(data->cmd, data))
			return (1);
		data->cmd = set_command_list(data->cmd);
		ret = set_values(data);
		if (ret == EXIT_FAILURE)
			return (close_pipe_fds(data->cmd, NULL), ret);
		return (handle_execute(data));
	}
	else
		ft_putstr_fd("No command found\n", 2);
	return (0);
}
