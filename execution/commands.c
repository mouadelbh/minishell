/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:21:30 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/23 13:54:36 by zelbassa         ###   ########.fr       */
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
	signal(SIGQUIT, SIG_DFL);
	if (command[0][0] == '/')
		path = ft_strdup(command[0]);
	else if (command[0][0] != '\0')
		path = get_full_cmd(command[0], envp);
	if (execve(path, command, envp) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit_status = 127;
		free(path);
		reset_shell(data, 0);
	}
	return (0);
}

int single_command(t_data *data)
{
	char	*path;

	data->cmd->cmd = NULL;
	data->cmd->io_fds = NULL;
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
			return (ft_putstr_fd("fork error\n", 2), 1);
		if (data->pid == 0)
			exit_status = exec_cmd(data->cmd->argv, data->envp_arr, data);
		waitpid(data->pid, &exit_status, 0);
		handle_child_term(exit_status);
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

t_io_fds	*dup_io(t_io_fds *io)
{
	t_io_fds	*new;

	new = (t_io_fds *)malloc(sizeof(t_io_fds));
	if (!new)
        return (NULL);
	new->in_fd = io->in_fd;
	new->out_fd = io->out_fd;
	new->heredoc_name = ft_strdup(io->heredoc_name);
	new->infile = ft_strdup(io->infile);
	new->outfile = ft_strdup(io->outfile);
	return (new);
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
	new->argv[i] = NULL;
	new->cmd = ft_strdup(src->cmd);
	new->type = src->type;
	new->pipe_fd[0] = src->pipe_fd[0];
	new->pipe_fd[1] = src->pipe_fd[1];
	new->pipe_output = src->pipe_output;
	new->io_fds = dup_io(src->io_fds);
	new->file_error = src->file_error;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	free_tmp_io(t_io_fds *io)
{
	if (io->infile)
    {
        free(io->infile);
        io->infile = NULL;
    }
	if (io->outfile)
	{
		free(io->outfile);
		io->outfile = NULL;
	}
	if (io->heredoc_name)
    {
		printf("The heredoc name is %s\n", io->heredoc_name);
		unlink(io->heredoc_name);
        free(io->heredoc_name);
        io->heredoc_name = NULL;
    }
	free(io);
}

void	set_cmd_list(t_cmd **cmd, t_cmd **new)
{
	t_cmd	*tmp;
	t_cmd	*current;

	tmp = *cmd;
	while (tmp)
	{
		current = tmp->next;
		if (tmp->type == CMD)
			lstadd_cmd(new, copy_node(tmp));
		if (tmp->io_fds)
			free_tmp_io(tmp->io_fds);
		free_cmd_node(tmp);
		tmp = current;
	}
}

int	complex_command(t_data *data)
{
	t_cmd	*new;
	t_cmd	*cmd;

	cmd = data->cmd;
	new = NULL;
	if (data->cmd)
	{
		if (!create_files(data->cmd, data))
			return (1);
		if (!create_pipes(data))
		{
			ft_putstr_fd("Failed to create pipes\n", 2);
			close_pipe_fds(data->cmd, NULL);
			return (EXIT_FAILURE);
		}
		if (set_values(data) == 1)
			return (close_pipe_fds(data->cmd, NULL), 1);
		set_cmd_list(&data->cmd, &new);
		data->cmd = new;
		return (handle_execute(data));
	}
	else
		ft_putstr_fd("No command found\n", 2);
	return (0);
}
