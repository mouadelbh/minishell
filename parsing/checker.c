/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:00 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 14:52:19 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	check_dollar(char *s, int i)
{
	if (!s[i + 1] || is_space(s[i + 1]) \
	|| s[i + 1] == '$' || quotes_open(s, i) == 1)
		return (1);
	if ((s[i + 1] == '\"' || s[i + 1] == '\'') && quotes_open(s, i))
		return (1);
	return (0);
}

int	check_token(int c)
{
	if (c == '|')
		return (1);
	if (c == '<' || c == '>')
		return (2);
	return (0);
}

int	checkspaces(char *line)
{
	if (is_empty(line))
	{
		g_exit_status = 0;
		return (free(line), 0);
	}
	return (1);
}

int	return_syntax_error(char *line, int i)
{
	if (quotes_open(line, i) == 1)
		return (ft_putstr_fd("minishell: syntax error near \
unexpected token '\''\n", 2), 0);
	if (quotes_open(line, i) == 2)
		return (ft_putstr_fd("minishell: syntax error near \
unexpected token '\"'\n", 2), 0);
	return (1);
}

int	checkquotes(char *line, t_data *data)
{
	int	i;
	int	count;
	int	hdoc;

	i = 0;
	hdoc = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == '<' && line[i + 1] == '<' && !quotes_open(line, i))
		{
			count++;
			hdoc++;
		}
		if (line[i] == '$' && !check_dollar(line, i) && !hdoc)
			line[i] = -1;
		i++;
	}
	if (count >= 17)
	{
		perror("minishell: maximum here-document count exceeded\n");
		reset_shell(data, 0);
	}
	return (return_syntax_error(line, i));
}
