/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:54 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 16:24:43 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_words(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (*(s + i))
	{
		while (*(s + i) == c && *(s + i))
			i++;
		if (*(s + i) != '\0')
		{
			count++;
			while (*(s + i) != c && *(s + i))
				i++;
		}
	}
	return (count);
}

int	isredir(int i)
{
	if (i == REDIR_IN || i == REDIR_OUT || i == APPEND)
		return (1);
	if (i == HEREDOC)
		return (2);
	return (0);
}

void	reset_shell(t_data *data, int i)
{
	if (i)
		ft_putstr_fd("exit\n", 1);
	free_all(data, 1);
	rl_clear_history();
	exit(g_exit_status);
}

int	quotes_open(char *s, int i)
{
	int	quotes[2];
	int	j;

	ft_bzero(quotes, 2 * 4);
	j = 0;
	while (s[j] && j < i)
	{
		if (s[j] == '\'' && !(quotes[1] % 2))
			quotes[0]++;
		else if (s[j] == '\"' && !(quotes[0] % 2))
			quotes[1]++;
		j++;
	}
	if (quotes[0] % 2)
		return (1);
	else if (quotes[1] % 2)
		return (2);
	else
		return (0);
}
