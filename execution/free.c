/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 02:55:16 by zelbassa          #+#    #+#             */
/*   Updated: 2024/11/19 02:15:27 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	free_all(t_data *data)
// {
// 	t_line	*temp;
// 	t_line	*next;

// 	temp = data->head;
// 	while (temp)
// 	{
// 		next = temp->next;
// 		free(temp->str);
// 		free(temp);
// 		temp = next;
// 	}
// }

void	free_io(t_io_fds *io_fds)
{
	if (io_fds)
	{
		if (io_fds->infile)
			free(io_fds->infile);
		if (io_fds->outfile)
			free(io_fds->outfile);
		if (io_fds->heredoc_name)
			free(io_fds->heredoc_name);
		free(io_fds);
	}
}

void	free_envp(t_list **head)
{
	t_list	*tmp;

	tmp = (*head);
	while (*head)
	{
		free((*head)->content);
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
}

void	free_all(t_data *data)
{
	// if (data->envp_arr)
	free_arr(data->envp_arr);
	free_envp(&data->envp);
	data->envp_arr = NULL;
}

void	free_data(t_data *data, int exit_code)
{
	// free_io(data->cmd->io_fds);
	// free_cmd(data->cmd);
	free_all(data);
	exit(exit_code);
}
