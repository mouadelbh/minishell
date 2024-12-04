/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:29 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/04 13:32:28 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_getenv(char *name, t_data *data)
{
	int 	i;
	t_list	*temp = data->envp;

	if (!name || !data || !data->envp)
		return NULL;
	i = 0;
	while (temp)
	{
		if (ft_strncmp(temp->content, name, ft_strlen(name)) == 0)
		{
			return (ft_strdup(temp->content));
		}
		temp = temp->next;
	}
	return (NULL);
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
	return (1);
}

int	builtin(char *cmd)
{
	char	*builtin[8];
	int		i;

	i = 0;
	builtin[0] = "echo";
	builtin[1] = "cd";
	builtin[2] = "pwd";
	builtin[3] = "export";
	builtin[4] = "unset";
	builtin[5] = "env";
	builtin[6] = "exit";
	builtin[7] = NULL;
	while (i < 7)
	{
		if (ft_strncmp(builtin[i], cmd, 0) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(t_data *data, char **cmd)
{
	int	res;

	res = 127;
	if (ft_strncmp(cmd[0], "pwd", 0) == 0)
		res = ft_pwd(data, cmd);
	else if (ft_strncmp(cmd[0], "env", 0) == 0)
		res = ft_env(data, cmd, 0);
	else if (ft_strncmp(cmd[0], "echo", 0) == 0)
	{
		// printf("Running echo\n");
		res = ft_echo(data, cmd);
	}
	else if (ft_strncmp(cmd[0], "cd", 0) == 0)
		res = ft_cd(data, cmd);
	else if (ft_strncmp(cmd[0], "unset", 0) == 0)
		res = ft_unset(data, cmd);
	else if (ft_strncmp(cmd[0], "export", 0) == 0)
		res = ft_export(data, cmd);
	else if (ft_strncmp(cmd[0], "exit", 0) == 0)
		res = ft_exit(data, cmd);
	return (res);
}

int	ft_error(int error, t_data *data)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error == 0)
		ft_putstr_fd("syntax error: cannot find a closing quote\n",
			STDERR_FILENO);
	else if (error == 1)
		ft_putstr_fd("error when forking\n",
			STDERR_FILENO);
	else if (error == 2)
		ft_putstr_fd("error finding command\n", STDERR_FILENO);
	else if (error == 3)
		ft_putstr_fd("Could not find corresponding path\n", STDERR_FILENO);
	else if (error == 4)
		ft_putstr_fd("Ambigius rediredirict\n", 2);
	else if (error == 7)
	{
		data->status = 127;
		ft_putstr_fd(data->cmd->argv[0], STDERR_FILENO);
		ft_putchar_fd(' ', 2);
		ft_putstr_fd("command not found\n", STDERR_FILENO);
	}
	return (EXIT_FAILURE);
}

void set_list_var(t_data *data, char *name, char *new_value)
{
	int 	len;
	t_list *current;
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
			if (!current->content)
			{
				free(temp);
				ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
				return;
			}
			free(temp);
			break;
		}
		current = current->next;
	}
}

int	count_pipes(t_data *data)
{
	int	i;
	int	k;
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
	// free(temp);
	return (k);
}

char	*get_full_cmd(char *av, char **env)
{
	int		i;
	char	*tmp;
	char	*full_cmd;
	char	**path;

	i = 0;
	(void)env;
	path = ft_split(getenv("PATH"), ':');
	if (!path)
		ft_error(3, NULL);
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
	free_arr(path);
	return (ft_strdup(av));
}

int	count_symbols(t_data *data)
{
	int		i;
	t_line	*temp = data->head;

	i = 0;
	while (temp)
	{
		if (temp->type >= 1 && temp->type <= 5)
			i++;
		temp = temp->next;
	}
	return (i);
}

char *ft_strcat(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*dest;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	len1 = strlen(s1);
	len2 = strlen(s2);
	dest = malloc(len1 + len2 + 1);
	if (!dest)
		return (NULL);
	memcpy(dest, s1, len1);
	memcpy(dest + len1, s2, len2);
	dest[len1 + len2] = '\0';
	return (dest);
}

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

char	*new_strjoin(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
	{
		if (s1)
			return (ft_strdup(s1));
		if (s2)
			return (ft_strdup(s2));
		else
			return (NULL);
	}
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	while (*s1)
		*result++ = *s1++;
	*result = ' ';
	result++;
	while (*s2)
		*result++ = *s2++;
	*result = '\0';
	return (result);
}

void set_cmd_strings(t_cmd *cmd)
{
	t_cmd	*current = cmd;
	int		i;
	char	*temp;
	char	*new_cmd;

	new_cmd = NULL;
	while (current != NULL)
	{
		i = 0;
		current->cmd = NULL;
		while (current->argv[i] != NULL)
		{
			temp = ft_strjoin(current->cmd, current->argv[i]);
			if (temp == NULL)
			{
				perror("Failed to allocate memory");
				exit(EXIT_FAILURE);
			}
			if (current->argv[i + 1] != NULL)
				current->cmd = ft_strjoin(temp, " ");
			else
			{
				if (current->cmd)
					free(current->cmd);
				current->cmd = ft_strdup(temp);
			}
			free(temp);
			i++;
		}
		current = current->next;
	}
}

char	**set_list_arra(t_list *env)
{
	char	**result;
	t_list	*temp = env;
	int		i;

	i = ft_lstsize(env);
	result = malloc(sizeof(char *) * (i + 1));
	if (!result)
		return NULL;
	result[i] = NULL;
	i = 0;
	while (temp)
	{
		result[i] = ft_strdup(temp->content);
		if (!result[i])
		{
			while (i > 0)
			{
				free(result[--i]);
			}
			free(result);
			return NULL;
		}
		temp = temp->next;
		i++;
	}
	return (result);
}
