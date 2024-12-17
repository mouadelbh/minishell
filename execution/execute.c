/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:28:55 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/17 02:45:43 by zelbassa         ###   ########.fr       */
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

int	execute_command(t_data *data, t_cmd *cmd)
{
	int	ret;

	if (cmd->type != CMD)
		exit(0);
	if (!set_pipe_fds(data->cmd, cmd) || !redirect_io(cmd->io_fds))
		return (1);
	close_fds(data->cmd, false);
	ret = exec_builtin(data, cmd->argv);
	if (ret != 127)
		exit(0);
	if (ret == 127)
		return (new_exec(cmd, data->envp_arr, data));
	return (ret);
}
