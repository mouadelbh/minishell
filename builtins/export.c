/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:47:56 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/12 20:49:07 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*new_substr(const char *str, int c)
{
	int		i;
	int		j;
	char	*result;

	j = 0;
	i = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != c)
		i++;
	result = malloc(sizeof(char) * (i + 1));
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[i] = '\0';
	return (result);
}

char	*find_value(char *name, t_list *envp)
{
	int		i;
	char	*key;
	t_list	*temp = envp;

	i = 0;
	key = new_substr(name, '=');
	while (temp)
	{
		if (ft_strncmp(key, temp->content, ft_strlen(key)) == 0)
			return (temp->content);
		temp = temp->next;
	}
	return (NULL);
}

static t_list	*create_env_node(const char *key)
{
	t_list *new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return NULL;
	new_node->content = strdup(key);
	if (!new_node->content)
		return (free(new_node), NULL);
	new_node->next = NULL;
	return new_node;
}

void create_env_value(t_data *data, char *key)
{
	t_list *new_node = create_env_node(key);
	if (!new_node)
		return;
	if (data->envp == NULL)
		data->envp = new_node;
	else
	{
		t_list *temp = data->envp;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
}

// void	list_env(t_data *data, char **cmd)
// {
// 	int	i;

// 	i = 0;
// 	while (data->envp_arr[i])
// 	{
// 		printf("declare -x %s\n", data->envp_arr[i]);
// 		i++;
// 	}
// }

int ft_export(t_data *data, char **cmd)
{
	char	*value;
	char	*env_var;
	char	*new_value;

	if (!cmd[1])
		ft_env(data, cmd);
	value = find_value(cmd[1], data->envp);
	if (!value)
		create_env_value(data, cmd[1]);
	else
	{
		env_var = new_substr(cmd[1], '=');
		new_value = ft_strchr(cmd[1], '=');
		modify_env_value(env_var, new_value + 1, data);
	}
	return (0);
}