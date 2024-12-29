/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triming.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:18:29 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 16:21:01 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static char	*camouflage(char *str)
{
	char	*tmp;

	tmp = malloc(2);
	tmp[0] = 31;
	tmp[1] = '\0';
	free(str);
	return (tmp);
}

int	should_skip_quote(char *str, int i)
{
	if (str[i] == '\'' && \
	(quotes_open(str, i) == 1 || quotes_open(str, i) == 0))
		return (1);
	else if (str[i] == '\"' && \
	(quotes_open(str, i) == 2 || quotes_open(str, i) == 0))
		return (1);
	return (0);
}

char	*trim(char *str, int type)
{
	char	*result;
	int		j;
	int		i;

	if (type == CMD && (!ft_strncmp(str, "\"\"", 2) \
	|| !ft_strncmp(str, "\'\'", 2)))
		return (camouflage(str));
	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (should_skip_quote(str, i))
		{
			i++;
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (free(str), result);
}

void	triming_quotes(t_line *head)
{
	int	i;

	while (head)
	{
		i = 0;
		while (head->str[i])
		{
			if (ft_strchr(head->str[i], '\'') || ft_strchr(head->str[i], '\"'))
				head->str[i] = trim(head->str[i], head->type);
			i++;
		}
		head = head->next;
	}
}
