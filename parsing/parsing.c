/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:58:54 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/18 08:22:43 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	exit_status;

int	print_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
	exit_status = 2;
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

void	flag_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]) && quotes_open(line, i))
			line[i] = 31;
		i++;
	}
}

void	unflag_spaces(char **line)
{
	int	i;
	int	j;

	i = 0;
	if (!line)
		return ;
	while (line[i])
	{
		j = 0;
		while (line[i][j])
		{
			if (line[i][j] == 31)
				line[i][j] = ' ';
			j++;
		}
		i++;
	}
}

void	update_env(t_cmd *cmd, t_data *data)
{
	char	*arg;
	char	*prev;
	int		i;
	t_cmd	*tmp;

	arg = NULL;
	prev = NULL;
	while (cmd->next)
		cmd = cmd->next;
	i = 0;
	while (cmd->argv[i] && !(check_token(cmd->argv[i][0]) == 2 && \
	cmd->argv[i + 1] && !cmd->argv[i + 2]))
	{
		arg = ft_strdup(cmd->argv[i]);
		if (prev)
			free(prev);
		prev = arg;
		i++;
	}
	if (!arg)
	{
		arg = malloc(1);
		arg[0] = '\0';
	}
	modify_env_value("_", arg, data);
	free(arg);
}

int	parse(char *str, t_line **head, t_list *env,t_data* ex_data)
{
	char	**arg;
	t_line *tmp;
	char	*line;
	t_line	*new;
	int		i;

	if (ex_data->arg == NULL)
		reset_shell(ex_data, 1);
	if (!checkspaces(str))
		return (-1);
	add_history(ex_data->arg);
	if (!checkquotes(str, ex_data))
	{
		exit_status = 2;
		return (-1);
	}
	line = spacing(str);
	flag_spaces(line);
	arg = ft_split(line, ' ');
	free(line);
	unflag_spaces(arg);
	if (!arg)
	{
		exit_status = 139;
		return (-1);
	}
	lexer(arg, head);
	expanding(head, env);
	triming_quotes(*head);
	return (parse_error(*head));
}
