/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:34:19 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/27 20:26:01 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	valid_command(t_cmd *cmd, t_data *data)
{
	char	*full_command;

	full_command = get_full_cmd(cmd->argv[0], data->envp_arr);
	if (!full_command)
		full_command = ft_strdup("");
	if (access(full_command, F_OK) == -1)
	{
		free(full_command);
		if (ft_strncmp(cmd->argv[0], "./", 2) == 0 \
			|| ft_strncmp(cmd->argv[0], "/", 1) == 0)
			ft_error(data, "No such file or directory", 127, 0);
		else
			ft_error(data, "command not found", 127, 0);
		return (0);
	}
	free(full_command);
	return (1);
}

int	check_cmd(t_data *data, char *cmd)
{
	struct stat	buf;

	if (!ft_strchr(cmd, '/'))
		return (0);
	if (stat(cmd, &buf) == -1)
		return (0);
	if (S_ISDIR(buf.st_mode))
	{
		ft_error(data, "Is a directory", 126, 0);
		return (1);
	}
	return (0);
}

int	check_permission(char *path, t_data *data)
{
	if (!path)
		return (0);
	if (access(path, X_OK) != 0 && access(path, F_OK) == 0)
	{
		ft_error(data, "Permission denied", 126, 0);
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
	if (check_cmd(data, cmd->argv[0]) || check_permission(full_cmd, data))
		return (free(full_cmd), 0);
	free(full_cmd);
	if (cmd->type == CMD)
		return (valid_command(cmd, data));
	return (0);
}

int	should_pipe(t_cmd *cmd)
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
