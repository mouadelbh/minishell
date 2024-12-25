/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/25 16:21:51 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_input(t_data *data)
{
	data->sym_count = count_symbols(data);
	if (data->sym_count == 0 && data->head->type == CMD)
		return (single_command(data));
	set_cmd_strings(data->cmd);
	return (complex_command(data));
}

static void	print_remaining_nodes(t_data *data)
{
	t_cmd *cmd;

	cmd = data->cmd;
	if (cmd)
		show_command_info(cmd);
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
		update_env(data->cmd, data);
		data->envp_arr = set_list_arra(data->envp);
		if (!data->envp_arr || !*data->envp_arr)
		{
			free_line(data->head);
			continue;
		}
		g_exit_status = handle_input(data);
		signal(SIGINT, handlesig);
		free_all(data, 0);
	}
	return (g_exit_status);
}
