/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triming.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:18:29 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/25 19:47:55 by mel-bouh         ###   ########.fr       */
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

char	*trim(char *str, int type)
{
	char	*result;
	int		j;
	int		i;
	int		in[2];

	if (type == CMD && (!ft_strncmp(str, "\"\"", 2) || !ft_strncmp(str, "\'\'", 2)))
		return (camouflage(str));
	ft_bzero(in, 8);
	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in[0])
		{
			in[1] = !in[1];
			i++;
			continue ;
		}
		else if (str[i] == '\"' && !in[1])
		{
			in[0] = !in[0];
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
