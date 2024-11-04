/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:48:20 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/04 02:44:32 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_exit(t_data *data, char **cmd)
{
	int	exit_code;

	exit_code = 0;
	printf("Exit\n");
	if (cmd[1])
	{
		if (cmd[2])
			return (ft_error(1, data));
		exit_code = ft_atoi(cmd[1]);
		if (exit_code < 0)
		{
			ft_error(2, data);
			exit_code = 255;
		}
	}
	exit(0);
	// return (0);
}