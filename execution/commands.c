/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/11/11 10:50:29 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	should_pipe(t_cmd *cmd)
{
	t_cmd	*temp = cmd;
	int		pipe_count = 0;

	while (temp)
	{
		if (temp->type == CMD)
			pipe_count++;
		temp = temp->next;
	}
	if (pipe_count % 2 == 0)
		return (1);
	return (0);
}

void	init_command(t_cmd *cmd, t_data *data)
{
	init_io(&cmd->io_fds);
	if (should_pipe(cmd) || (cmd->next && cmd->next->type == CMD))
		cmd->pipe_output = true;
}

int	handle_execute(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmd;
	while (data->pid != 0 && cmd)
	{
		if (cmd->type == CMD)
		{
			data->pid = fork();
			if (data->pid == -1)
				ft_putstr_fd("fork error\n", 2);
		}
		if (data->pid == 0)
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
	else if (cmd[0][0] != '\0')
	{
		path = get_full_cmd(cmd[0], env);
	}
	if (!path)
		return (ft_error(7, data), 1);
	if (execve(path, cmd, env) == -1)
	{
		perror("execve");
		global.g_exit_status = GENERAL_ERROR;
		return (1);
	}
	return (0);
}

int	single_command(t_data *data, char *cmd)
{
	t_line	*temp = data->head;

	while (temp)
	{
		if (temp->next && temp->next->type == 7)
			temp = temp->next;
		if (builtin(data->cmd->argv[0]))
			exec_builtin(data, data->cmd->argv);
		else
		{
			data->pid = fork();
			if (data->pid == -1)
				return (ft_error(1, data));
			if (data->pid == 0)
			{
				data->status = exec_cmd(cmd, data->envp_arr, data);
			}
			waitpid(0, NULL, 0);
		}
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

t_cmd	*set_command_list(t_cmd *cmd)
{
	t_cmd	*new_list;
	t_cmd	*current;
	t_cmd	*temp;

	if (!cmd)
		return (NULL);
	while (cmd && cmd->type != CMD)
		cmd = cmd->next;
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
		create_files(data->cmd, data);
		data->cmd = set_command_list(data->cmd);
		ret = set_values(data);
		return (handle_execute(data));
	}
	else
		ft_putstr_fd("No command found\n", 2);
	return (0);
}
