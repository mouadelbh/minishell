/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/04 17:59:28 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_input(t_data *data)
{
	t_line	*temp = data->head;
	char	*cmd;

	data->sym_count = count_symbols(data);
	if (data->sym_count == 0 && data->head->type == CMD)
	{
		cmd = NULL;
		init_io(&data->cmd->io_fds);
		data->status = single_command(data, cmd);
	}
	else
	{
		set_cmd_strings(data->cmd);
		complex_command(data);
	}
	return (0);
}

int	minishell(t_data *data)
{
	t_line	*head;
	t_cmd	*cmd;
	int		new_fd;
	int		err;

	while (!data->exit)
	{
		data->head = NULL;
		data->cmd = NULL;
		data->arg = readline(READLINE_MSG);
		data->pid = -1;
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
			printf("CMD IS NULL\n");
			continue;
		}
		data->envp_arr = set_list_arra(data->envp);
		if (!data->envp_arr || !*data->envp_arr)
		{
			free_line(data->head);
			continue;
		}
		data->status = handle_input(data);
		free_all(data);
	}
	return (data->status);
}
