/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:28:55 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/17 05:54:54 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	set_values(t_data *data)
{
	if (!data || !data->cmd)
		return (EXIT_SUCCESS);
	if (!data->cmd->cmd)
	{
		if (data->cmd->io_fds
			&& !check_infile_outfile(data->cmd->io_fds))
		{
			ft_putstr_fd("Error in the fds\n", 2);
			return (EXIT_FAILURE);
		}
		ft_putstr_fd("No command\n",2 );
		return (EXIT_SUCCESS);
	}
	if (!create_pipes(data))
	{
		ft_putstr_fd("Failed to create pipes\n", 2);
		return (EXIT_FAILURE);
	}
	return (127);
}

int	new_exec(t_cmd *cmd, char **envp, t_data *data)
{
	char	*path;

	if (!command_is_valid(data, cmd, builtin(cmd->argv[0])))
		exit(exit_status);
	if (cmd->argv[0][0] == '/')
		path = ft_strdup(cmd->argv[0]);
	else if (cmd->argv[0][0] != '\0')
		path = get_full_cmd(cmd->argv[0], envp);
	if (cmd->next && cmd->next->file_error == 0)
	{
		free(path);
		exit(126);
	}
	if (execve(path, cmd->argv, envp) == -1)
	{
		ft_putstr_fd("execve failed\n", 2);
		return (free(path), 127);
	}
	return (0);
}

void	print_ios(t_cmd *cmd)
{
	int fd = open(ft_itoa(getpid()), O_RDWR | O_APPEND | O_CREAT, 0644);
	ft_putstr_fd("cmd: ", fd);
	ft_putstr_fd(cmd->cmd, fd);
	ft_putchar_fd('\n', fd);
	if (cmd->io_fds)
	{
		ft_putstr_fd("in_fd: ", fd);
		ft_putnbr_fd(cmd->io_fds->in_fd, fd);
		ft_putstr_fd(" infile: ", fd);
		ft_putstr_fd(cmd->io_fds->infile, fd);
		ft_putchar_fd('\n', fd);
		ft_putstr_fd("out_fd: ", fd);
		ft_putnbr_fd(cmd->io_fds->out_fd, fd);
		ft_putstr_fd(" outfile: ", fd);
		ft_putstr_fd(cmd->io_fds->outfile, fd);
		ft_putchar_fd('\n', fd);
	}
	if (cmd->pipe_fd)
	{
		ft_putstr_fd("pipe_fd[0]: ", 2);
		ft_putnbr_fd(cmd->pipe_fd[0], 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("pipe_fd[1]: ", 2);
		ft_putnbr_fd(cmd->pipe_fd[1], 2);
		ft_putchar_fd('\n', 2);
	}
		// while (cmd)
	// {
	// 	ft_putstr_fd("Current command: ", 2);
	// 	ft_putstr_fd(cmd->cmd, 2);
	// 	ft_putchar_fd('\n', 2);
	// 	cmd = cmd->next;
	// }
	// cmd = temp;
	close(fd);
}

int	execute_command(t_data *data, t_cmd *cmd)
{
	int	ret;

	if (cmd->type != CMD)
		exit(0);
	if (!set_pipe_fds(data->cmd, cmd) || !redirect_io(cmd->io_fds))
		return (1);
	// print_ios(cmd);
	close_fds(data->cmd, false);
	ret = exec_builtin(data, cmd->argv);
	if (ret != 127)
		exit(0);
	if (ret == 127)
		return (new_exec(cmd, data->envp_arr, data));
	return (ret);
}
