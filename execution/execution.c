/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/22 15:34:41 by mel-bouh         ###   ########.fr       */
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

int	handle_input(t_data *data)
{
	t_line	*temp = data->head;
	char	*cmd;

	data->cmd_count = count_symbols(data);
	if (data->cmd_count == 0 && data->head->type == CMD)
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
		// for (t_line *tmp = data->head; tmp; tmp = tmp->next)
		// {
		// 	printf("this is a head node \n");
		// 	printf("---------------\n");
		// 	for (int i = 0; tmp->str[i]; i++)
		// 		printf("str[%i] = %s\n", i, tmp->str[i]);
		// 	printf("type = %i\n", tmp->type);
		// }
		get_final_list(&data->head, &data->cmd);
		printf("%p\n", data->cmd);
		for (t_cmd *tmp = data->cmd; tmp;tmp = tmp->next)
		{
			printf("this is a node \n");
			printf("---------------\n");
			for (int i = 0;tmp->argv[i];i++)
				printf("str[%i] = %s\n", i, tmp->argv[i]);
			printf("type = %i\n", tmp->type);
		}
		data->envp_arr = set_list_arra(data->envp);
		data->status = handle_input(data);
		free_line(&data->head);
		free_all(data);
	}
	return (data->status);
}
