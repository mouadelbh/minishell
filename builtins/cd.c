/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:43:22 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/07 14:09:39 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Still need some adjustments */

int	cd_error(char *path, int error)
{
	ft_putstr_fd("cd: ", 2);
	if (path)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
	}
	if (error == -2)
		ft_putstr_fd("not a directory\n", 2);
	else
		ft_putendl_fd(strerror(errno), 2);
	return (1);
}

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
		return (cd_error(NULL, 0));
	if (arg[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	valid_path = is_valid_path(arg[1]);
	if (valid_path == -1)
		return (cd_error(arg[1], 0));
	else if (valid_path == -2)
		return (cd_error(arg[1], -2));
	else if (valid_path == -3)
		return (cd_error(arg[1], 0));
	old_pwd = getcwd(NULL, 0);
	if (chdir(arg[1]) == -1)
		return (cd_error(arg[1], 0));
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (cd_error(NULL, 0));
	modify_env_value("PWD", new_pwd, data);
	modify_env_value("OLDPWD", old_pwd, data);
	free(new_pwd);
	return (0);
}
