/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:12:14 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/26 16:13:01 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handledoc(int sig)
{
	(void)sig;
	g_exit_status = CTRL_C;
	close(0);
}

char	*expand_string(char *line, t_list *envp)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && line[i + 1] != '$')
			line[i] = -1;
		i++;
	}
	line = find_and_replace(line, envp, 0);
	return (line);
}

void	execute_signal(t_data *data, int temp_fd, char *temp_file)
{
	if (g_exit_status == -1)
	{
		ft_putstr_fd("minishell: warning: here-document delimited by \
end-of-file\n", 2);
		g_exit_status = 0;
	}
	unlink(temp_file);
	free(temp_file);
	ft_close(temp_fd);
	reset_shell(data, 0);
}
