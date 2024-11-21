/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/21 22:33:49 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_cmd_list(t_cmd **cmd)
{
	int		i;
	t_cmd	*temp;

	i = 0;
	while (*cmd)
	{
		i = 0;
		while ((*cmd)->argv[i])
			free((*cmd)->argv[i++]);
		free((*cmd)->argv);
		temp = *cmd;
		*cmd = (*cmd)->next;
		free(temp);
	}
}

static int	count_commands(t_data *data)
{
	int		count;
	t_line	*temp;

	count = 0;
	temp = data->head;
	while (temp)
	{
		if (temp->type == CMD)
			count++;
		temp = temp->next;
	}
	return (count);
}

int	handle_input(t_data *data)
{
	t_line	*temp = data->head;
	char	*cmd;

	data->sym_count = count_symbols(data);
	data->cmd_count = count_commands(data);
	if (data->sym_count == 0 && data->head->type == CMD)
	{
		// if (data->cmd)
		// 	cmd = to_str(data->cmd->argv);
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

void	free_cmd(t_cmd **cmd)
{
	int		i;

	i = 0;
	while (*cmd)
	{
		i = 0;
		while ((*cmd)->argv[i])
			free((*cmd)->argv[i++]);
		free((*cmd)->argv);
		free(*cmd);
		*cmd = (*cmd)->next;
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
		data->envp_arr = set_list_arra(data->envp);
		data->status = handle_input(data);
		free_line(&data->head);
		free_all(data);
	}
	return (data->status);
}
