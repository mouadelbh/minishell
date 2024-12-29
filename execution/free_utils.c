/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:37:02 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/21 16:15:27 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void free_cmd_node(t_cmd *cmd)
// {
// 	if (cmd)
// 	{
// 		if (cmd->argv)
// 			free_arr(cmd->argv);
// 		if (cmd->cmd)
// 			free(cmd->cmd);
// 		if (cmd->pipe_fd)
// 			free(cmd->pipe_fd);
// 		if (cmd->io_fds)
// 			free(cmd->io_fds);
// 		free_io(cmd);
// 		free(cmd);
// 	}
// }
