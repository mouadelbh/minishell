/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:58:54 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/11 08:23:51 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	print_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
	g_exit_status = 2;
	return (-1);
}

int	redir_error(t_line *head)
{
	while (head)
	{
		if (check_token(head->str[0][0]) == 2 && !head->next)
			return (print_error("newline"));
		if (check_token(head->str[0][0]) == 2 && head->next->type == PIPE)
			return (print_error("|"));
		if (check_token(head->str[0][0]) == 2 && check_token(head->next->str[0][0]) == 2)
			return (print_error(head->next->str[0]));
		head = head->next;
	}
	return (0);
}

int	pipe_error(t_line *head)
{
	if (head->type == PIPE)
		return (1);
	while (head)
	{
		if (head->type == PIPE && !head->next)
			return (1);
		if (head->type == PIPE && head->next->type == PIPE)
			return (1);
		head = head->next;
	}
	return (0);
}

int	parse_error(t_line *head)
{
	if (pipe_error(head))
		return (print_error("|"));
	if (redir_error(head))
		return (-1);
	return (0);
}

int	parse(char *str, t_line **head, t_parse *data, t_data* ex_data)
{
	char	**arg;
	char	*line;

	if (ex_data->arg == NULL)
		reset_shell(ex_data);
	if (!checkspaces(str))
		return (-1);
	if (!checkquotes(str))
	{
		g_exit_status = 2;
		return (-1);
	}
	add_history(ex_data->arg);
	line = spacing(str);
	line = find_and_replace(line, data->env);
	arg = ft_split(line, ' ');
	free(line);
	if (!arg)
	{
		g_exit_status = 139;
		return (-1);
	}
	lexer(arg, head, data);
	triming_quotes(*head);
	return (parse_error(*head));
}
