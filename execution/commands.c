/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2024/12/23 11:40:34 by zelbassa         ###   ########.fr       */
=======
/*   Updated: 2024/12/23 11:35:52 by zelbassa         ###   ########.fr       */
>>>>>>> Stashed changes
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
		data->pid = fork();
		if (data->pid != -1)
			signal(SIGINT, handlehang);
		if (data->pid == -1)
			return (ft_putstr_fd("fork error\n", 2), 1);
		if (data->pid == 0)
			execute_command(data, cmd);
		cmd = cmd->next;
	}
	cmd = temp;
	close_file(data, cmd);
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

int single_command(t_data *data)
{
	t_line *temp;
	char	*path;

	temp = data->head;
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
		free(path);
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
	update_env(data->cmd, data);
	return (exit_status);
}

static void	lstadd_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_cmd	*copy_node(t_cmd *src)
{
	t_cmd	*new;
	int		i;

	i = 0;
	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	while (src->argv[i])
		i++;
	new->argv = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (src->argv[i])
	{
		new->argv[i] = ft_strdup(src->argv[i]);
		i++;
	}
	new->cmd = ft_strdup(src->cmd);
	// printf("The command is: %s\n", new->cmd);
	new->type = src->type;
	new->pipe_fd[0] = src->pipe_fd[0];
	new->pipe_fd[1] = src->pipe_fd[1];
	new->pipe_output = src->pipe_output;
	new->io_fds = src->io_fds;
	new->file_error = src->file_error;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	set_cmd_list(t_cmd **cmd, t_cmd **new)
{
	t_cmd	*tmp;

	tmp = *cmd;
	while (tmp)
	{
		if (tmp->type == CMD)
			lstadd_cmd(new, copy_node(tmp));
		tmp = tmp->next;
	}
}

int	complex_command(t_data *data)
{
	t_line	*temp;
	t_cmd	*new;
	t_cmd	*cmd;
	int		ret;

	cmd = data->cmd;
	new = NULL;
	temp = data->head;
	if (data->cmd)
	{
		if (!create_files(data->cmd, data))
			return (1);
<<<<<<< Updated upstream
		data->cmd = set_command_list(data->cmd);
		ret = set_values(data);
		if (ret == EXIT_FAILURE)
			return (close_pipe_fds(data->cmd, NULL), ret);
=======
		if (!create_pipes(data))
		{
			ft_putstr_fd("Failed to create pipes\n", 2);
			return (EXIT_FAILURE);
		}
		// ret = set_values(data);
		// if (ret == EXIT_FAILURE)
		// 	return (close_pipe_fds(data->cmd, NULL), ret);
		set_cmd_list(&data->cmd, &new);
		// data->cmd = set_command_list(cmd);
		data->cmd = new;
		// show_command_info(data->cmd);
>>>>>>> Stashed changes
		return (handle_execute(data));
	}
	else
		ft_putstr_fd("No command found\n", 2);
	return (0);
}
