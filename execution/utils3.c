/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:09:26 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/26 17:59:39 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*to_str(char **arr)
{
	char	*result;
	char	*temp;
	int		i;

	i = 0;
	result = NULL;
	while (arr[i])
	{
		temp = ft_strjoin(result, arr[i]);
		free(result);
		if (arr[i + 1])
			result = ft_strjoin(temp, " ");
		else
			result = ft_strjoin(temp, "");
		free(temp);
		i++;
	}
	return (result);
}

char	**set_list_arra(t_list *env)
{
	char	**result;
	t_list	*temp;
	int		i;

	temp = env;
	i = ft_lstsize(env);
	result = malloc(sizeof(char *) * (i + 1));
	if (!result)
		return (NULL);
	result[i] = NULL;
	i = 0;
	while (temp)
	{
		result[i] = ft_strdup(temp->content);
		if (!result[i])
		{
			while (i > 0)
				free(result[--i]);
			free(result);
			return (NULL);
		}
		temp = temp->next;
		i++;
	}
	return (result);
}

int	modify_env_value(char *name, char *new_value, t_data *data)
{
	char	*str;
	char	*temp;
	char	*temp2;

	str = ft_getenv(name, data);
	if (!str)
	{
		temp = ft_strjoin(name, "=");
		temp2 = ft_strjoin(temp, new_value);
		create_env_value(data, temp2, 0);
		free(temp);
		free(temp2);
	}
	else
		set_list_var(data, name, new_value);
	free(str);
	return (1);
}

void	set_list_var(t_data *data, char *name, char *new_value)
{
	int		len;
	t_list	*current;
	char	*temp;

	len = ft_strlen(name);
	current = data->envp;
	while (current)
	{
		if (ft_strncmp(current->content, name, len) == 0)
		{
			free(current->content);
			current->content = NULL;
			temp = ft_strjoin(name, "=");
			current->content = ft_strjoin(temp, new_value);
			current->empty_value = 0;
			free(temp);
			if (!current->content)
			{
				ft_putstr_fd("Error: Memory allocation failed\n", 2);
				return ;
			}
			break ;
		}
		current = current->next;
	}
}
