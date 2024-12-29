/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:29 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/27 13:19:41 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*construct_full_cmd(char *path, char *av)
{
	char	*tmp;
	char	*full_cmd;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full_cmd = ft_strjoin(tmp, av);
	free(tmp);
	if (!full_cmd)
		return (NULL);
	if (access(full_cmd, X_OK | F_OK) == 0)
		return (full_cmd);
	free(full_cmd);
	return (NULL);
}

static char	*get_path(char **env)
{
	int		i;
	char	*result;

	i = 0;
	result = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			result = ft_strdup(env[i] + 5);
			break ;
		}
		i++;
	}
	return (result);
}

char	*get_full_cmd(char *av, char **env)
{
	int		i;
	char	**path;
	char	*env_path;
	char	*full_cmd;

	if (ft_strncmp(av, ".", 1) == 0)
		return (ft_strdup(av));
	env_path = get_path(env);
	if (!env_path || av[0] == '\0')
		return (free(env_path), NULL);
	path = ft_split(env_path, ':');
	free(env_path);
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
	{
		full_cmd = construct_full_cmd(path[i], av);
		if (full_cmd)
			return (free_arr(path), full_cmd);
		i++;
	}
	return (free_arr(path), NULL);
}

static char	*concat_cmd_strings(char **argv)
{
	int		i;
	char	*cmd;
	char	*temp;

	i = 0;
	cmd = NULL;
	while (argv[i] != NULL)
	{
		temp = ft_strjoin(cmd, argv[i]);
		if (!temp)
			return (NULL);
		if (argv[i + 1] != NULL)
		{
			free(cmd);
			cmd = ft_strjoin(temp, " ");
		}
		else
		{
			free(cmd);
			cmd = ft_strdup(temp);
		}
		free(temp);
		i++;
	}
	return (cmd);
}

void	set_cmd_strings(t_cmd *cmd)
{
	while (cmd != NULL)
	{
		cmd->cmd = concat_cmd_strings(cmd->argv);
		cmd->file_error = 1;
		cmd = cmd->next;
	}
}
