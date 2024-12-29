/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:43:22 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/19 09:45:16 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Still need some adjustments */

int	is_valid_path(char *path)
{
	struct stat	buf;

	if (stat(path, &buf) == -1)
		return (-1);
	if (!S_ISDIR(buf.st_mode))
		return (-2);
	if (access(path, X_OK) == -1)
		return (-3);
	return (0);
}

int	ft_cd(t_data *data, char **arg)
{
	char	*old_pwd;
	char	*new_pwd;
	int		valid_path;

	if (!arg[1])
		return (ft_putendl_fd("cd: no such file or directory", 2), 1);
	if (arg[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	valid_path = is_valid_path(arg[1]);
	if (valid_path == -1)
		return (ft_putstr_fd("cd: no such file or directory\n", 2), 1);
	else if (valid_path == -2)
		return (ft_putstr_fd("cd: not a directory\n", 2), 1);
	else if (valid_path == -3)
		return (ft_putstr_fd("cd: permission denied\n", 2), 1);
	old_pwd = getcwd(NULL, 0);
	if (chdir(arg[1]) == -1)
		return (perror("cd"), 1);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (perror("getcwd"), 1);
	modify_env_value("PWD", new_pwd, data);
	modify_env_value("OLDPWD", old_pwd, data);
	return (free(old_pwd), free(new_pwd), 0);
}
