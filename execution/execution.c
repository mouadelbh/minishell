/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/18 08:36:07 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_input(t_data *data)
{
	t_line	*temp = data->head;
	char	*cmd;
	int		i;

	data->sym_count = count_symbols(data);
	if (data->sym_count == 0 && data->head->type == CMD)
	{
		cmd = NULL;
		init_io(&data->cmd->io_fds);
		return (single_command(data, cmd));
	}
	else
	{
		set_cmd_strings(data->cmd);
		return (complex_command(data));
	}
	return (0);
}

int	minishell(t_data *data)
{
	int		err;
	t_cmd	*cmd;
	t_line	*head;
	int		new_fd;

	while (1)
	{
		data->head = NULL;
		data->cmd = NULL;
		data->arg = readline(PROMPT);
		data->pid = -1;
		data->envp_arr = NULL;
		err = parse(data->arg, &data->head, data->envp, data);
		if (err == -1)
		{
			free_line(data->head);
			continue;
		}
		get_final_list(&data->head, &data->cmd);
		if (!data->cmd)
		{
			free_line(data->head);
			continue;
		}
		// update_env(data->cmd, data);
		data->envp_arr = set_list_arra(data->envp);
		// show_env(data->envp_arr, data->envp);
		if (!data->envp_arr || !*data->envp_arr)
		{
			free_line(data->head);
			continue;
		}
		exit_status = handle_input(data);
		exit_status = exit_status;
		reset_signal();
		free_all(data, 0);
	}
	return (exit_status);
}
