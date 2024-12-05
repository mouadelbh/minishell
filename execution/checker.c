/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:34:19 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/04 15:35:21 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	valid_command(t_cmd *cmd, t_data *data)
{
	char	*full_command;

	full_command = get_full_cmd(cmd->argv[0], data->envp_arr);
	if (access(full_command, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free(full_command);
		data->status = 1;
		return (0);
	}
	free(full_command);
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
		data->status = 126;
		return (1);
	}
	return (0);
}

int	command_is_valid(t_data *data, t_cmd *cmd, int is_builtin)
{
	char	*full_cmd;

	if (is_builtin)
		return (1);
	full_cmd = get_full_cmd(cmd->argv[0], data->envp_arr);
	if (check_cmd(cmd->argv[0], data) || check_permission(full_cmd, data))
		return (free(full_cmd), 0);
	free(full_cmd);
	if (cmd->type == CMD)
		return (valid_command(cmd, data));
	return (0);
}

int should_pipe(t_cmd *cmd)
{
	t_cmd	*temp;
	
	int		pipe_count;

	pipe_count = 0;
	temp = cmd;
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
