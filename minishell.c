/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:18:31 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/25 16:22:56 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_exit_status = 0;

void	handlehang(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
	g_exit_status = CTRL_C;
}

void	handlesig(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = CTRL_C;
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
	return (g_exit_status);
}
