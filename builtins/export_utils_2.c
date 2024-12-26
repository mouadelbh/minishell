/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 09:43:26 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 16:42:42 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_value(char *name, t_list *envp)
{
	char	*key;
	t_list	*temp;
	char	*result;
	char	*value;

	temp = envp;
	if (!is_valid_env_name(name))
		return (NULL);
	key = new_substr(name, '=');
	if (!key)
		return (NULL);
	while (temp)
	{
		value = new_substr(temp->content, '=');
		if (ft_strncmp(key, value, ft_strlen(value) == 0))
		{
			result = ft_strdup(temp->content);
			free(key);
			free(value);
			return (result);
		}
		free(value);
		temp = temp->next;
	}
	return (free(key), NULL);
}

static t_list	*create_env_node(const char *key, int empty_value)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup(key);
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
