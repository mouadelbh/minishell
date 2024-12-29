/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 18:25:46 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/29 20:37:09 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_execute(t_data *data)
{
	t_cmd	*cmd;
	t_cmd	*temp;

	cmd = data->cmd;
	temp = cmd;
	while (cmd)
	{
		data->pid = fork();
		if (data->pid != -1)
		{
			signal(SIGINT, handlehang);
			if (ft_strnstr(data->cmd->argv[0], "./minishell", ft_strlen(data->cmd->argv[0])))
				signal(SIGINT, SIG_IGN);
		}
		if (data->pid == -1)
			return (ft_putstr_fd("fork error\n", 2), 1);
		if (data->pid == 0)
			execute_command(data, cmd);
		cmd = cmd->next;
	}
	cmd = temp;
	close_file(data, cmd);
	return (g_exit_status);
}

int	exec_cmd(char **command, char **envp, t_data *data)
{
	char	*path;

	path = NULL;
	signal(SIGQUIT, SIG_DFL);
	if (command[0][0] == '/')
		path = ft_strdup(command[0]);
	else if (command[0][0] != '\0')
		path = get_full_cmd(command[0], envp);
	if (!path)
		path = ft_strdup("");
	if (execve(path, command, envp) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
		free(path);
		reset_shell(data, 0);
	}
	return (0);
}

int	single_command(t_data *data)
{
	char	*path;

	data->cmd->cmd = NULL;
	data->cmd->io_fds = NULL;
	if (builtin(data->cmd->argv[0]))
		g_exit_status = exec_builtin(data, data->cmd->argv);
	else
	{
		path = get_full_cmd(data->cmd->argv[0], data->envp_arr);
		if (check_cmd(data->cmd->argv[0]) == 1 \
			|| check_permission(path, data) == 1)
		{
			free(path);
			g_exit_status = 126;
			return (126);
		}
		free(path);
		if (!fork_and_exec(data))
			return (0);
	}
	update_env(data->cmd, data);
	return (g_exit_status);
}
