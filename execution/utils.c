/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:29 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 15:44:50 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	count_pipes(t_data *data)
{
	int		i;
	int		k;
	t_line	*temp;

	i = 0;
	k = 0;
	temp = data->head;
	while (temp)
	{
		while (temp->str[i])
		{
			if (temp->type == 1)
				k++;
			i++;
		}
		temp = temp->next;
	}
	return (k);
}

char	*get_full_cmd(char *av, char **env)
{
	int		i;
	char	*tmp;
	char	*full_cmd;
	char	**path;
	char	*env_path;

	i = 0;
	(void)env;
	env_path = getenv("PATH");
	if (!env_path || av[0] == '\0')
		return (NULL);
	path = ft_split(env_path, ':');
	if (!path)
		return (NULL);
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (free_arr(path), NULL);
		full_cmd = ft_strjoin(tmp, av);
		free(tmp);
		if (!full_cmd)
			return (free_arr(path), NULL);
		if (access(full_cmd, X_OK | F_OK) == 0)
			return (free_arr(path), full_cmd);
		free(full_cmd);
		i++;
	}
	return (free_arr(path), ft_strdup(av));
}

void	set_cmd_strings(t_cmd *cmd)
{
	int		i;
	char	*temp;

	while (cmd != NULL)
	{
		i = 0;
		cmd->cmd = NULL;
		while (cmd->argv[i] != NULL)
		{
			temp = ft_strjoin(cmd->cmd, cmd->argv[i]);
			if (temp == NULL)
				perror("Failed to allocate memory");
			if (cmd->argv[i + 1] != NULL)
			{
				free(cmd->cmd);
				cmd->cmd = ft_strjoin(temp, " ");
			}
			else
			{
				if (cmd->cmd)
				{
					free(cmd->cmd);
					cmd->cmd = NULL;
				}
				cmd->cmd = ft_strdup(temp);
			}
			free(temp);
			i++;
		}
		cmd->file_error = 1;
		cmd = cmd->next;
	}
}
