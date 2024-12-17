/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:28:55 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/17 08:47:55 by zelbassa         ###   ########.fr       */
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

// int	should_run(t_cmd *current)
// {
// 	t_cmd	*cmd;

// 	cmd = current->next;
// 	if (!cmd)
// 		return (1);
// 	if (cmd->type == APPEND || cmd->type == REDIR_OUT
// 		|| cmd->type == REDIR_IN || cmd->type == HEREDOC)
// 	{
// 		// ft_putstr_fd("Reached here\n", 2);
// 		return (cmd->file_error);
// 	}
// 	return (1);
// }

int	new_exec(t_cmd *cmd, char **envp, t_data *data)
{
	char	*path;

	if (!command_is_valid(data, cmd, builtin(cmd->argv[0])))
		exit(exit_status);
	if (cmd->argv[0][0] == '/')
		path = ft_strdup(cmd->argv[0]);
	else if (cmd->argv[0][0] != '\0')
		path = get_full_cmd(cmd->argv[0], envp);
	// if (cmd->next && (cmd->next->type >= 2 && cmd->next->type <= 4) && cmd->next->file_error == 0)
	// {
	// 	ft_putstr_fd("Next type is ", 2);
	// 	ft_putnbr_fd(cmd->next->type, 2);
	// 	ft_putstr_fd("\n", 2);
	// 	ft_putstr_fd("Next file error is ", 2);
	// 	ft_putnbr_fd(cmd->next->file_error, 2);
	// 	ft_putstr_fd("\n", 2);
	// 	ft_putstr_fd("Skipping command\n", 2);
	// 	free(path);
	// 	exit(126);
	// }
	if (cmd->next)
	{
		ft_putstr_fd("Current command is ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd("\n", 2);
		ft_putstr_fd("--------------------\n", 2);
		ft_putstr_fd("Next command is ", 2);
		ft_putstr_fd(cmd->next->cmd, 2);
		ft_putstr_fd("\n", 2);
		ft_putstr_fd("there is a next command\n", 2);
		if (cmd->next->type >= 2 && cmd->next->type <= 4)
		{
			ft_putstr_fd("Command is of type redirection\n", 2);
			ft_putnbr_fd(cmd->next->type, 2);
			ft_putstr_fd("\n", 2);
			if (cmd->next->file_error == 0)
			{
				ft_putstr_fd("File error is 0\n", 2);
				ft_putstr_fd("Skipping command\n", 2);
				free(path);
				exit(126);
			}
			else
			{
				ft_putstr_fd("File error is not 0\n", 2);
			}
		}
		else
		{
			ft_putstr_fd("Not a redirection command\n", 2);
		}
	}
	if (execve(path, cmd->argv, envp) == -1)
	{
		ft_putstr_fd("execve failed\n", 2);
		return (free(path), 127);
	}
	return (0);
}

int	execute_command(t_data *data, t_cmd *cmd)
{
	int	ret;

	if (cmd->type != CMD)
		exit(0);
	set_pipe_fds(data->cmd, cmd);
	redirect_io(cmd->io_fds);
	close_fds(data->cmd, false);
	ret = exec_builtin(data, cmd->argv);
	if (ret != 127)
		exit(0);
	if (ret == 127)
		return (new_exec(cmd, data->envp_arr, data));
	return (ret);
}
