/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:54 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/21 17:56:31 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

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

void	handlesig(int sig)
{
	if (global.pid == 0)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_redisplay();
		global.g_exit_status = CTRL_C;
	}
	else
	{
		ft_putstr_fd("\n", 1);
		global.g_exit_status = CTRL_C;
	}
}

int is_empty(char *str)
{
	int i;

	i = 0;
	while (str[i] && isspace(str[i]))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	free_env(t_list *env)
{
	t_list	*temp;
	t_list	*next;

	temp = env;
	while (temp)
	{
		next = temp->next;
		free(temp->content);
		free(temp);
		temp = next;
	}
}

void	reset_shell(t_data *data)
{
	ft_putstr_fd("exit\n", 1);
	free_env(data->envp);
	data->status = 1;
	exit(global.g_exit_status);
}

int	isspace(int c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
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
	else if (quotes[1] % 2 || quotes[0] % 2)
		return (2);
	else
		return (0);
}
