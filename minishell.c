/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:18:31 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/08 20:38:49 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_exit_status;

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		i++;
	}
	free(arr);
}

t_list	*set_env(char **envp)
{
	t_list	*env;
	t_list	*new;
	int		i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		new = malloc(sizeof(t_list));
		if (!new)
			return (NULL);
		new->content = ft_strdup(envp[i]);
		new->next = env;
		env = new;
		i++;
	}
	return (env);
}

void	free_list(t_list *env)
{
	t_list	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->content);
		free(env);
		env = tmp;
	}
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	t_data	program;

	if (ac != 1)
		return (printf("minishell doesn't take any arguments\n"), 1);
	program.envp = set_env(env);
	program.envp_arr = set_list_arra(program.envp);
	program.exit = 0;
	program.status = 0;
	minishell(&program);
	free_list(program.envp);
	free_arr(program.envp_arr);
	return (0);
}
