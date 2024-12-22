/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:56 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/20 21:18:06 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	create_env_value(t_data *data, char *key, int empty_value)
{
	t_list	*new;
	char	*value;

	value = NULL;
	if (!empty_value)
		value = ft_strdup("");
	new = ft_lstnew(key);
	if (!new)
	{
		free(value);
		ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
		return ;
	}
	if (!empty_value)
	{
		free(new->content);
		new->content = ft_strjoin(key, "=");
		if (!new->content)
		{
			ft_lstclear(&new, free);
			free(value);
			ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
			return ;
		}
	}
	ft_lstadd_back(&data->envp, new);
	free(value);
}

// int	export_error(char *cmd)
// {
// 	ft_putstr_fd("minishell: export: `", 2);
// 	ft_putstr_fd(cmd, 2);
// 	ft_putstr_fd("': not a valid identifier\n", 2);
// 	return (1);
// }

// int	add_entry(char *cmd, char *value, t_data *data)
// {
// 	if (!ft_strchr(cmd, '=') && is_valid_env_name(cmd)
// 		&& !value)
// 		return (create_env_value(data, cmd, 1), 0);
// 	if (value && (!ft_strchr(cmd, '=') || !(ft_strchr(cmd, '=') + 1)))
// 		return (0);
// 	return (1);
// }

// static int	handle_append(char *key, char *env_value, char **cmd, t_data *data)
// {
// 	if (!append_env_value(key, env_value, cmd[1], data->envp))
// 	{
// 		printf("Reached here\n");	
// 		modify_env_value(key, env_value, data);
// 	}
// 	return (0);
// }

// static int	handle_export(char *key, char *env_value, char **cmd, t_data *data)
// {
// 	char	*value;

// 	value = find_value(key, data->envp);
// 	if (!value)
// 		create_env_value(data, cmd[1], 0);
// 	else
// 		modify_env_value(key, env_value, data);
// 	free(value);
// 	return (0);
// }

int	ft_export(t_data *data, char **cmd)
{
	// char	*key;
	// char	*env_value;
	// int		append;
	// char	*value;

	// append = 0;
	// if (!cmd[1])
	// 	return (ft_env(data, cmd, 1));
	// if (!is_valid_env_name(cmd[1]))
	// 	return (export_error(cmd[1]));
	// value = find_value(cmd[1], data->envp);
	// if (!add_entry(cmd[1], value, data))
	// 	return (free(value), 0);
	// free(value);
	// key = get_key(cmd[1], data->envp, &env_value, &append);
	// if (append)
	// 	handle_append(key, env_value, cmd, data);
	// else
	// 	handle_export(key, env_value, cmd, data);
	// free(key);
	// free(env_value);
	return (0);
}
