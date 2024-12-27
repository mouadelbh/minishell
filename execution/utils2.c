/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:06:28 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/27 14:14:35 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getenv(char *name, t_data *data)
{
	t_list	*temp;

	temp = data->envp;
	if (!name || !data || !data->envp)
		return (NULL);
	while (temp)
	{
		if (ft_strncmp(temp->content, name, ft_strlen(name)) == 0)
			return (ft_strdup(temp->content));
		temp = temp->next;
	}
	return (NULL);
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
		res = ft_pwd(cmd);
	else if (ft_strncmp(cmd[0], "env", 0) == 0)
		res = ft_env(data, cmd, 0);
	else if (ft_strncmp(cmd[0], "echo", 0) == 0)
		res = ft_echo(cmd);
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

int	count_symbols(t_data *data)
{
	int		i;
	t_line	*temp;

	temp = data->head;
	i = 0;
	while (temp)
	{
		if (temp->type >= 1 && temp->type <= 5)
			i++;
		temp = temp->next;
	}
	return (i);
}

char	*ft_strcat(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*dest;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	dest = malloc(len1 + len2 + 1);
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s1, len1);
	ft_memcpy(dest + len1, s2, len2);
	dest[len1 + len2] = '\0';
	return (dest);
}
