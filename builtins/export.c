/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:56 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/04 13:42:57 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_value(char *name, t_list *envp)
{
	int		i;
	char	*key;
	t_list	*temp;
	char	*result;

	temp = envp;
	i = 0;
	if (is_valid_env_name(name) == 0)
		return (NULL);
	key = new_substr(name, '=');
	while (temp)
	{
		if (ft_strncmp(key, temp->content, ft_strlen(key)) == 0)
		{
			result = strdup(temp->content);
			free(key);
			return (result);
		}
		temp = temp->next;
	}
	free(key);
	return (NULL);
}

static t_list	*create_env_node(const char *key, int empty_value)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = strdup(key);
	new_node->empty_value = empty_value;
	if (!new_node->content)
		return (free(new_node), NULL);
	new_node->next = NULL;
	return (new_node);
}

int	is_valid_env_name(char *var)
{
	int	i;

	i = 0;
	if (ft_isalpha(var[i]) == 0 && var[i] != '_')
		return (0);
	i++;
	while (var[i] && var[i] != '=')
	{
		if (var[i] == '+' && var[i + 1] && var[i + 1] == '=')
			return (2);
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	create_env_value(t_data *data, char *key, int empty_value)
{
	t_list	*new_node;
	t_list	*temp;

	new_node = create_env_node(key, empty_value);
	if (!new_node)
		return ;
	if (data->envp == NULL)
		data->envp = new_node;
	else
	{
		temp = data->envp;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
}

int	ft_export(t_data *data, char **cmd)
{
	char	*key;
	char	*env_value;
	int		append;

	append = 0;
	if (!cmd[1])
		return (ft_env(data, cmd, 1));
	if (!is_valid_env_name(cmd[1]))
	{
		printf("minishell: export: '%s': not a valid identifier\n", cmd[1]);
		return (1);
	}
	if (!ft_strchr(cmd[1], '=') && is_valid_env_name(cmd[1]))
		create_env_value(data, cmd[1], 1);
	key = get_key(cmd[1], data->envp, &env_value, &append);
	if (append)
	{
		if (!append_env_value(key, env_value, cmd[1], data->envp))
			modify_env_value(key, env_value, data);
	}
	free(key);
	free(env_value);
	return (0);
}
