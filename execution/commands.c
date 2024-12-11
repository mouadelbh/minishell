/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/10 12:03:25 by zelbassa         ###   ########.fr       */
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
			data->pid = fork();
			if (data->pid != -1)
				signal(SIGINT, handlehang);
			if (data->pid == -1)
				return (ft_putstr_fd("fork error\n", 2), 1);
		}
		if (data->pid == 0)
			exit_status = execute_command(data, cmd);
		cmd = cmd->next;
	}
	waitpid(0, &data->status, 0);
	data->status = WEXITSTATUS(exit_status);
	return (close_file(data), exit_status);
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

void	modify_last_arg(t_data *data)
{
	int	i;

	i = 0;
	while (data->cmd->argv[i])
		i++;
	modify_env_value("_", data->cmd->argv[i - 1], data);
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
		{	
			data->status = exec_builtin(data, data->cmd->argv);
		}
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
				data->status = exec_cmd(data->cmd->argv, data->envp_arr, data);
			waitpid(data->pid, &data->status, 0);
			data->status = WEXITSTATUS(data->status);
		}
		// free(path);
		temp = temp->next;
	}
	modify_last_arg(data);
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
