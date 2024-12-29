/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 11:45:30 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/26 14:00:33 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_env(t_list *envp)
{
	if (envp == NULL)
		return ;
	while (envp)
	{
		if (!envp->empty_value)
		{
			if (!ft_strncmp(envp->content, "_=", 2))
				printf("_=/usr/bin/env\n");
			else
				printf("%s\n", (char *)envp->content);
		}
		envp = envp->next;
	}
}

int	ft_env(t_data *data, char **args, int export)
{
	t_list	*env_temp;

	(void)args;
	env_temp = data->envp;
	if (!data->envp)
		return (EXIT_FAILURE);
	if (export == 0)
		print_env(env_temp);
	else
	{
		while (env_temp)
		{
			printf("declare -x %s\n", (char *)env_temp->content);
			env_temp = env_temp->next;
		}
	}
	return (EXIT_SUCCESS);
}
