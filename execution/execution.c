/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/26 18:14:57 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	close_file(t_data *data, t_cmd *cmd)
{
	pid_t	wpid;
	int		status;

	close_fds(cmd);
	wpid = 0;
	status = 0;
	while (wpid != -1 || errno != ECHILD)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == data->pid)
			handle_child_term(status);
	}
	return (g_exit_status);
}

void	handle_child_term(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			if (WCOREDUMP(status))
				ft_putstr_fd("Quit: core dumped\n", STDERR_FILENO);
			else
				ft_putstr_fd("Quit\n", STDERR_FILENO);
		}
		g_exit_status = 128 + WTERMSIG(status);
	}
	else
		g_exit_status = WEXITSTATUS(status);
}

int	handle_input(t_data *data)
{
	data->sym_count = count_symbols(data);
	if (data->sym_count == 0 && data->head->type == CMD)
		return (single_command(data));
	set_cmd_strings(data->cmd);
	return (complex_command(data));
}

void	init_args(t_data *data)
{
	data->head = NULL;
	data->cmd = NULL;
	data->arg = readline(PROMPT);
	data->pid = -1;
	data->envp_arr = NULL;
}

int	minishell(t_data *data)
{
	while (1)
	{
		init_args(data);
		if (parse(data->arg, &data->head, data->envp, data) == -1)
		{
			free_line(data->head);
			continue ;
		}
		get_final_list(&data->head, &data->cmd);
		if (!data->cmd)
		{
			free_line(data->head);
			continue ;
		}
		update_env(data->cmd, data);
		data->envp_arr = set_list_arra(data->envp);
		g_exit_status = handle_input(data);
		signal(SIGINT, handlesig);
		free_all(data, 0);
	}
	return (g_exit_status);
}
