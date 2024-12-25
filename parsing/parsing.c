/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:58:54 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/25 16:37:13 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	redir_error(t_line *head)
{
	while (head)
	{
		if (isredir(head->type) && !head->next)
			return (print_error("newline"));
		if (isredir(head->type) && head->next->type == PIPE)
			return (print_error("|"));
		if (isredir(head->type) && isredir(head->next->type))
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

void	expanding(t_line **head, t_list *env)
{
	t_line	*new;
	int		flag;
	int		i;

	new = *head;
	flag = 0;
	while (new)
	{
		i = 0;
		if (isredir((*head)->type))
		{
			new = new->next;
			flag = 1;
		}
		if (!new)
			break ;
		while (new && new->str[i])
		{
			new->str[i] = find_and_replace(new->str[i], env, flag);
			i++;
			flag = 0;
		}
		new = new->next;
	}
}

int	parse(char *str, t_line **head, t_list *env, t_data *ex_data)
{
	t_line	*tmp;
	t_line	*new;
	char	**arg;
	char	*line;
	int		i;

	if (ex_data->arg == NULL)
		reset_shell(ex_data, 1);
	if (!checkspaces(str))
		return (-1);
	add_history(ex_data->arg);
	if (!checkquotes(str, ex_data))
		return (set_exit_status(2));
	line = spacing(str);
	flag_spaces(line);
	arg = ft_split(line, ' ');
	free(line);
	unflag_spaces(arg);
	if (!arg)
		return (set_exit_status(1));
	lexer(arg, head);
	expanding(head, env);
	triming_quotes(*head);
	return (parse_error(*head));
}
