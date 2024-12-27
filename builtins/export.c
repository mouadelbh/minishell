/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:56 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/26 16:42:12 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	export_error(char *cmd)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	add_entry(char *cmd, char *value, t_data *data)
{
	char	*equal_sign;

	equal_sign = ft_strchr(cmd, '=');
	if (!equal_sign && is_valid_env_name(cmd) && !value)
		return (create_env_value(data, cmd, 1), 0);
	if (value && (!equal_sign))
		return (0);
	return (1);
}

static int	handle_export(char *cmd, t_data *data)
{
	char	*value;
	char	*key;
	char	*env_value;

	if (!is_valid_env_name(cmd))
		return (export_error(cmd));
	value = find_value(cmd, data->envp);
	if (!add_entry(cmd, value, data))
		return (free(value), 0);
	free(value);
	key = get_key(cmd, &env_value);
	value = find_value(key, data->envp);
	if (!value)
		create_env_value(data, cmd, 0);
	else
		modify_env_value(key, env_value, data);
	free(value);
	free(key);
	free(env_value);
	return (0);
}

int	ft_export(t_data *data, char **cmd)
{
	if (!cmd[1])
		return (ft_env(data, cmd, 1));
	g_exit_status = handle_export(cmd[1], data);
	if (cmd[2])
		ft_export(data, cmd + 1);
	return (g_exit_status);
}
