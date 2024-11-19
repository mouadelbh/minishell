/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:58:54 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/19 02:27:37 by mel-bouh         ###   ########.fr       */
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
	global.g_exit_status = 2;
	return (-1);
}

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
		if (isredir((*head)->type) == 2)
		{
			new = new->next;
			flag = 1;
		}
		while (new && new->str[i])
		{
			new->str[i] = find_and_replace(new->str[i], env, flag);
			i++;
			flag = 0;
		}
		new = new->next;
	}
}

int	parse(char *str, t_line **head, t_parse *data, t_data* ex_data)
{
	char	**arg;
	t_line *tmp;
	char	*line;
	t_line	*new;
	int		i;

	if (ex_data->arg == NULL)
		reset_shell(ex_data);
	if (!checkspaces(str))
		return (-1);
	add_history(ex_data->arg);
	if (!checkquotes(str))
	{
		global.g_exit_status = 2;
		return (-1);
	}
	line = spacing(str);
	arg = ft_split(line, ' ');
	free(line);
	if (!arg)
	{
		global.g_exit_status = 139;
		return (-1);
	}
	lexer(arg, head, data);
	expanding(head, data->env);
	triming_quotes(*head);
	tmp = *head;
	while (tmp)
	{
		printf("this is a node\n");
		printf("----------------\n");
		for (int i = 0;tmp->str[i];i++)
			printf("%s\n", tmp->str[i]);
		printf("%d\n", tmp->type);
		tmp = tmp->next;
	}
	return (parse_error(*head));
}
