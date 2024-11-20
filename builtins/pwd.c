/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:27 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/20 09:37:06 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_pwd(t_data *data, char **cmd)
{
	char	buff[MAX_PATH];

	// if (data->cmd->next && data->cmd->next->type == CMD)
	// 	return (printf("pwd: too many arguments\n"), EXIT_FAILURE);
	if (getcwd(buff, sizeof(buff)) != NULL)
		ft_putendl_fd(buff, 1);
	else
	{
		perror("getcwd");
		data->status = 0;
	}
	return (EXIT_FAILURE);
}