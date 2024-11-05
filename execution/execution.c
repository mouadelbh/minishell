/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/05 00:01:23 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_input(t_data *data)
{
	t_line	*temp = data->head;
	char	*cmd;

	int i = count_symbols(data);
	if (i == 0)
	{
		cmd = array_to_string(temp);
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
	t_parse	p_data;
	t_cmd	*cmd;
	int		new_fd;

	while (1)
	{
		head = NULL;
		cmd = NULL;
		data->arg = readline(READLINE_MSG);
		data->envp_arr = set_list_arra(data->envp);
		// if (data->arg == NULL || data->arg[0] == '\0')
		// 	reset_shell(data);
		add_history(data->arg);
		parse(data->arg, &head, data->envp_arr, &p_data);
		data->head = head;
		data->pid = -1;
		get_final_list(&head, &cmd);
		data->cmd = cmd;
		data->status = handle_input(data);
		new_fd = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
		ft_putnbr_fd(data->status, new_fd);
		exit_status = data->status;
	}
	return (data->status);
}
