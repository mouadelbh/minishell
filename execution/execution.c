/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/12 13:52:49 by zelbassa         ###   ########.fr       */
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
	t_line	*temp = data->head;
	char	*cmd;

	data->cmd_count = count_symbols(data);
	if (data->cmd_count == 0 && data->head->type == CMD)
	{
		if (data->cmd)
			cmd = to_str(data->cmd->argv);
		data->status = single_command(data, cmd);
	}
	else
	{
		set_cmd_strings(data->cmd);
		complex_command(data);
	}
	return (0);
}

void	free_cmd(t_cmd **head)
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
		// head = data->head;
		// while (head)
		// {
		// 	printf("this is a node\n");
		// 	printf("----------------\n");
		// 	for (int i = 0;head->str[i];i++)
		// 		printf("%s\n", head->str[i]);
		// 	printf("%d\n", head->type);
		// 	head = head->next;
		// }
		get_final_list(&data->head, &data->cmd);
		// cmd = data->cmd;
		// while (cmd)
		// {
		// 	printf("this is a node\n");
		// 	printf("----------------\n");
		// 	for (int i = 0;cmd->argv[i];i++)
		// 		printf("%s\n", cmd->argv[i]);
		// 	printf("%d\n", cmd->type);
		// 	cmd = cmd->next;
		// }
		data->envp_arr = set_list_arra(data->envp);
		data->status = handle_input(data);
		free_line(&data->head);
		free_all(data);
	}
	return (data->status);
}
