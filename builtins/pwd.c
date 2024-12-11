/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:27 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/12 00:49:10 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(t_data *data, char **cmd)
{
	char	buff[MAX_PATH];

	if (getcwd(buff, sizeof(buff)) != NULL)
		ft_putendl_fd(buff, 1);
	else
	{
		perror("getcwd");
		exit_status = 1;
	}
	return (exit_status);
}
