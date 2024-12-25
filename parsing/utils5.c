/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 15:30:03 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 16:37:19 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	set_exit_status(int status)
{
	g_exit_status = status;
	return (-1);
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i] && is_space(str[i]))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

int	is_space(int c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

void	update_env(t_cmd *cmd, t_data *data)
{
	char	*arg;
	char	*prev;
	int		i;

	arg = NULL;
	prev = NULL;
	while (cmd->next)
		cmd = cmd->next;
	i = 0;
	while (cmd->argv[i] && !(check_token(cmd->argv[i][0]) == 2 && \
	cmd->argv[i + 1] && !cmd->argv[i + 2]))
	{
		arg = ft_strdup(cmd->argv[i]);
		if (prev)
			free(prev);
		prev = arg;
		i++;
	}
	if (!arg)
	{
		arg = malloc(1);
		arg[0] = '\0';
	}
	modify_env_value("_", arg, data);
	free(arg);
}
