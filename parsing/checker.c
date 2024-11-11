/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:00 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/11 10:50:29 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	check_dollar(char *s, int i)
{
	if (!s[i + 1] || isspace(s[i + 1]) || s[i + 1] == '$' || quotes_open(s, i) == 1)
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
	int		i;

	i = 0;
	while (is_space(line[i]))
		i++;
	if (!line[i])
	{
		global.g_exit_status = 0;
		return (free(line), 0);
	}
	return (1);
}

int	checkquotes(char *line)
{
	int	i;
	int	hdoc;

	i = 0;
	hdoc = 0;
	while (line[i])
	{
		if (line[i] == '<' && line[i + 1] == '<' && !quotes_open(line, i))
			hdoc++;
		if (line[i] == '$' && !check_dollar(line ,i) && !hdoc)
			line[i] = -1;
		i++;
	}
	if (quotes_open(line, i) == 1)
		return (ft_putstr_fd("minishell: syntax error near unexpected token '\''\n", 2),0);
	if (quotes_open(line, i) == 2)
		return (ft_putstr_fd("minishell: syntax error near unexpected token '\"'\n", 2),0);
	return (1);
}
