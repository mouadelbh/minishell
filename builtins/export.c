/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:56 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/22 20:50:52 by zelbassa         ###   ########.fr       */
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
	if (!ft_strchr(cmd, '=') && is_valid_env_name(cmd)
		&& !value)
		return (create_env_value(data, cmd, 1), 0);
	if (value && (!ft_strchr(cmd, '=') || !(ft_strchr(cmd, '=') + 1)))
		return (0);
	return (1);
}

static int	handle_append(char *key, char *env_value, char **cmd, t_data *data)
{
	if (!append_env_value(key, env_value, cmd[1], data->envp))
	{
		printf("Reached here\n");	
		modify_env_value(key, env_value, data);
	}
	return (0);
}

static int	handle_export(char *key, char *env_value, char **cmd, t_data *data)
{
	char	*value;

	value = find_value(key, data->envp);
	if (!value)
		create_env_value(data, cmd[1], 0);
	else
		modify_env_value(key, env_value, data);
	free(value);
	return (0);
}

int	ft_export(t_data *data, char **cmd)
{
	char	*key;
	char	*env_value;
	int		append;
	char	*value;

	append = 0;
	if (!cmd[1])
		return (ft_env(data, cmd, 1));
	if (!is_valid_env_name(cmd[1]))
		return (export_error(cmd[1]));
	value = find_value(cmd[1], data->envp);
	if (!add_entry(cmd[1], value, data))
		return (free(value), 0);
	free(value);
	key = get_key(cmd[1], data->envp, &env_value, &append);
	if (append)
		handle_append(key, env_value, cmd, data);
	else
		handle_export(key, env_value, cmd, data);
	free(key);
	free(env_value);
	return (0);
}
