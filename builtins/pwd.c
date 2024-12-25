/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:27 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/25 14:52:19 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(char **cmd)
{
	char	buff[MAX_PATH];

	(void)cmd;
	if (getcwd(buff, sizeof(buff)) != NULL)
		ft_putendl_fd(buff, 1);
	else
	{
		perror("getcwd");
		g_exit_status = 1;
	}
	return (g_exit_status);
}
