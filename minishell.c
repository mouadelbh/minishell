/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:18:31 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/05 15:36:07 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	exit_status;

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr && !*arr)
		return ;
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
	arr = NULL;
}

static void	free_env(t_list **envp)
{
	t_list	*tmp;

	while (*envp)
	{
		tmp = (*envp)->next;
		free((*envp)->content);
		free(*envp);
		*envp = tmp;
	}
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	t_data	program;

	if (ac != 1)
		return (printf("minishell doesn't take any arguments\n"), 1);
	program.envp = NULL;
	init(&program.envp, env);
	program.status = 0;
	program.exit = 0;
	minishell(&program);
	free_env(&program.envp);
	return (exit_status);
}
