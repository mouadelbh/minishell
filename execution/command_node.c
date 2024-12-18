/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:36:32 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/18 11:19:40 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	lstadd_cmd(t_cmd **head, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

static t_cmd	*copy_node(t_cmd *node)
{
	t_cmd	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(t_cmd));
	while (node->argv[i])
		i++;
	new->argv = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (node->argv[i])
	{
		new->argv[i] = ft_strdup(node->argv[i]);
		i++;
	}
	new->argv[i] = NULL;
	new->cmd = ft_strdup(node->cmd);
	new->io_fds = malloc(sizeof(t_io_fds));
	new->io_fds->in_fd = node->io_fds->in_fd;
	new->io_fds->out_fd = node->io_fds->out_fd;
	new->io_fds->infile = ft_strdup(node->io_fds->infile);
	new->io_fds->outfile = ft_strdup(node->io_fds->outfile);
	new->io_fds->heredoc_name = ft_strdup(node->io_fds->heredoc_name);
	new->io_fds->stdin_backup = node->io_fds->stdin_backup;
	new->io_fds->stdout_backup = node->io_fds->stdout_backup;
	new->pipe_fd = NULL;
	new->pipe_output = node->pipe_output;
	printf("node->pipe_output = %p\n", &node->pipe_output);
	printf("node->pipe_output = %d\n", node->pipe_output);
	new->file_error = node->file_error;
	new->type = node->type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	set_command_list(t_cmd **cmd, t_cmd **new)
{
	// t_cmd	*new_list;
	// t_cmd	*current;
	t_cmd	*temp;
	t_cmd	*next_cmd;
	// t_cmd	*prev_cmd;

	
	temp = *cmd;
	while (temp)
	{
		// printf("this is a node\n");
		// printf("-------------\n");
		// for (int i = 0; temp->argv[i]; i++)
		// 	printf("temp = %s\n", temp->argv[i]);
		temp = temp->next;
	}
	temp = *cmd;
	while (temp)
	{
		if (temp->type == CMD)
			lstadd_cmd(new, copy_node(temp));
		temp = temp->next;
	}
	temp = *new;
	printf("new == %p\n", *new);
	while (temp)
	{
		// printf("this is a node\n");
		// printf("-------------\n");
		// for (int i = 0; temp->argv[i]; i++)
		// 	printf("temp = %s\n", temp->argv[i]);
		next_cmd = temp->next;
		free_cmd_node(temp);
		temp = next_cmd;
	}
	// printf("new not segv\n");
	temp = *cmd;
}