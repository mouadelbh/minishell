/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:29 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 18:01:28 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*get_full_cmd(char *av, char **env)
{
	int		i;
	char	**path;
	char	*env_path;
	char	*full_cmd;

	(void)env;
	env_path = getenv("PATH");
	if (!env_path || av[0] == '\0')
		return (NULL);
	path = ft_split(env_path, ':');
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
	return (free_arr(path), ft_strdup(av));
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
		if (temp == NULL)
			return (perror("Failed to allocate memory"), NULL);
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
		if (cmd->cmd == NULL)
		{
			perror("Failed to allocate memory");
			return ;
		}
		cmd->file_error = 1;
		cmd = cmd->next;
	}
}
