/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:28:55 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/05 15:35:51 by mel-bouh         ###   ########.fr       */
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
		ft_putstr_fd("No cd\n",2 );
		return (EXIT_SUCCESS);
	}
	if (!create_pipes(data))
	{
		ft_putstr_fd("Failed to create pipes\n", 2);
		return (EXIT_FAILURE);
	}
	return (127);
}

int	new_exec(char **command, char **envp, t_data *data)
{
	char	*path;

	if (command[0][0] == '/')
		path = ft_strdup(command[0]);
	else if (command[0][0] != '\0')
		path = get_full_cmd(command[0], envp);
	data->status = 0;
	if (execve(path, command, envp) == -1)
	{
		exit_status = 1;
		free(path);
		return (1);
	}
	return (0);
}

int	execute_command(t_data *data, t_cmd *cmd)
{
	int	ret;

	if (cmd->type != CMD)
		return (1);
	set_pipe_fds(data->cmd, cmd);
	redirect_io(cmd->io_fds);
	close_fds(data->cmd, false);
	ret = exec_builtin(data, cmd->argv);
	if (ret != 127)
		exit(0);
	if (ret == 127)
		return (new_exec(cmd->argv, data->envp_arr, data));
	return (ret);
}
