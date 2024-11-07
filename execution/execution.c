/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/07 22:55:08 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_cmd_list(t_cmd **head)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	tmp = (*head);
	while (*head)
	{
		i = 0;
		while ((*head)->argv[i])
			free((*head)->argv[i++]);
		free((*head)->argv);
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
}

int	handle_input(t_data *data)
{
	t_line	*temp;
	char	*cmd;
	int		i;

	temp = data->head;
	i = count_symbols(data);
	if (i == 0)
	{
		if (data->cmd)
			cmd = to_str(data->cmd->argv);
		if (!cmd)
			return (1);
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
		if (data->arg)
			add_history(data->arg);
		p_data.env = data->envp;
		parse(data->arg, &head, &p_data);
		data->head = head;
		data->pid = -1;
		get_final_list(&head, &cmd);
		data->cmd = cmd;
		while (cmd)
		{
			printf("  this is a node\n");
			printf("------------------\n");
			for (int i = 0;cmd->argv[i];i++)
				printf("str: %s\n", cmd->argv[i]);
			printf("type: %i\n", cmd->type);
			cmd = cmd->next;
		}
		data->status = handle_input(data);
		new_fd = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
		ft_putnbr_fd(data->status, new_fd);
		g_exit_status = data->status;
		free_line(&head);
		free_cmd_list(&cmd);
	}
	return (data->status);
}
