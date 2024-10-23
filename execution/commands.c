/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/10/23 22:19:53 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_command(t_cmd *cmd, t_data *data)
{
	init_io(&cmd->io_fds);
	if (cmd->next && cmd->next->type == CMD)
	{
		cmd->pipe_output = true;
	}
}

int	handle_execute(t_data *data)
{
	t_cmd	*cmd;
	int		pid;

	cmd = data->cmd;
	while (data->pid != 0 && cmd)
	{
		data->pid = fork();
		if (data->pid == -1)
			ft_putstr_fd("fork error\n", 2);
		else if (data->pid == 0)
			execute_command(data, cmd);
		cmd = cmd->next;
	}
	return (close_file(data));
}

int	exec_cmd(char *av, char **env, t_data *data)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(av, ' ');
	if (cmd[0][0] == '/')
		path = ft_strdup(cmd[0]);
	else
	{
		if (builtin(cmd[0]))
		{
			return (exec_builtin(data, cmd));
		}
		path = get_full_cmd(cmd[0], env);
	}
	if (!path)
		perror(cmd[0]);
	if (execve(path, cmd, env) == -1)
	{
		perror("execve");
		return (1);
	}
	return (0);
}

int	single_command(t_data *data, char *cmd)
{
	t_line	*temp = data->head;
	int pid;

	while (temp)
	{
		if (temp->next && temp->next->type == 7)
			temp = temp->next;
		data->pid = fork();
		if (data->pid == -1)
			return (ft_error(1, data));
		if (data->pid == 0)
		{
			exec_cmd(cmd, data->envp_arr, data);
		}
		waitpid(0, NULL, 0);
		temp = temp->next;
	}
	return (0);
}

void	complex_command(t_data *data)
{
	t_line	*temp = data->head;

	if (data->cmd)
	{
		create_files(data->cmd, data);
		execute_cmds(data->cmd, data->envp_arr, data);
	}
	else
		ft_putstr_fd("No command found\n", 2);
}