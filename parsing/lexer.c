/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:58:49 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/25 14:50:49 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	lstadd_line(t_line **head, t_line *new)
{
	t_line	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_token	get_token(char *str)
{
	if (!str)
		return (NONE);
	if (!ft_strncmp(str, "|", 1))
		return (PIPE);
	else if (!ft_strncmp(str, ">>", 2))
		return (APPEND);
	else if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	else if (!ft_strncmp(str, "<", 1))
		return (REDIR_IN);
	else if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUT);
	else
		return (NONE);
}

int	is_command(char **arg, int i, int *flag)
{
	if (i == 0)
		return (1);
	else if (*flag)
	{
		*flag = 0;
		return (1);
	}
	else if (get_token(arg[i - 1]) == PIPE)
		return (1);
	else
		return (0);
}

void	init_node(t_line **tmp)
{
	*tmp = malloc(sizeof(t_line));
	if (!*tmp)
		return ;
	(*tmp)->next = NULL;
	(*tmp)->prev = NULL;
}

void	lexer(char **arg, t_line **head)
{
	t_line	*tmp;
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	while (arg[i])
	{
		init_node(&tmp);
		lstadd_line(head, tmp);
		if (!tmp)
			return ;
		if (is_command(arg, i, &flag) && !check_token(arg[i][0]))
			tokenize_cmd(arg[i++], tmp);
		else if (ft_strchr(arg[i], '\"'))
			tokenize_quotarg(arg, &i, tmp);
		else if (ft_strchr(arg[i], '\''))
			tokenize_quotarg(arg, &i, tmp);
		else if (get_token(arg[i]) != NONE)
			tokenize(arg[i++], tmp);
		else
			tokenize_arg(arg, &i, tmp, &flag);
	}
	ft_free(arg);
}
