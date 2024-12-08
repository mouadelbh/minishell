/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:48:20 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/09 00:01:29 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_num(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	get_code(char *arg, int *error)
{
	unsigned long long	exit_code;

	if (!arg)
	{
		printf("No more arguments\n");
		return (exit_status);
	}
	exit_code = 0;
	if (!is_num(arg))
	{
		exit_code = 255;
	}
	else if (ft_atoi(arg) > 0 && ft_atoi(arg) < 256)
	{
		exit_code = ft_atoi(arg);
	}
	else if (ft_atoi(arg) > 255 || ft_atoi(arg) < 0)
	{
		exit_code = ft_atoi(arg) % 256;
		if (exit_code < 0)
			exit_code *= -1;
	}
	return (exit_code);
}

static int	arg_count(char *arg1, char *arg2)
{
	if (arg1 && arg2)
	{
		ft_putstr_fd("exit\nminishell: exit: too many arguments\n", 2);
		if (is_num(arg1))
			return (0);
		else
			printf("exit\n");
	}
	return (1);
}

int	ft_exit(t_data *data, char **cmd)
{
	int	exit_code;
	int	error;

	exit_code = 0;
	if (!cmd[1])
		printf("exit\n");
	if (!arg_count(cmd[1], cmd[2]))
		return (1);
	else if (cmd[1] && !cmd[2] && !is_num(cmd[1]))
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		data->status = 2;
		exit_code = 255;
	}
	if (is_num(cmd[1]))
		exit_code = get_code(cmd[1], &error);
	else if (!cmd[1])
		exit_code = data->status;
	else
		exit_code = 1;
	free_data(data, exit_code);
	exit(exit_code);
	return (0);
}
