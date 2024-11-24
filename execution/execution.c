/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/23 20:18:58 by zelbassa         ###   ########.fr       */
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

static void show_cmd(t_cmd *cmd)
{
	t_cmd	*temp;

	temp = cmd;
	if (!temp)
		printf("cmd is NULL\n");
	while (temp)
	{
		printf("cmd: %s\n", temp->cmd);
		printf("type: %d\n", temp->type);
		printf("pipe_output: %d\n", temp->pipe_output);
		show_command_ios(temp);
		temp = temp->next;
	}
}

int	minishell(t_data *data)
{
	t_line	*head;
	t_cmd	*cmd;
	t_parse	p_data;
	int		new_fd;
	int		err;

	while (!data->exit)
	{
		data->head = NULL;
		data->cmd = NULL;
		data->arg = readline(READLINE_MSG);
		p_data.env = data->envp;
		data->pid = -1;
		err = parse(data->arg, &data->head, &p_data, data);
		if (err == -1)
		{
			free_line(&data->head);
			continue;
		}
		get_final_list(&data->head, &data->cmd);
		if (!data->cmd)
		{
			free_line(&data->head);
			printf("CMD IS NULL\n");
			continue;
		}
		data->envp_arr = set_list_arra(data->envp);
		data->status = handle_input(data);
		free_line(&data->head);
		free_all(data);
	}
	return (data->status);
}
