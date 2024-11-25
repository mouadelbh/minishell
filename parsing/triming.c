/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triming.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:18:29 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/25 15:22:48 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	check_trim(char *str, int i)
{
	if (str[i] == '\"' && (quotes_open(str, i) != 1))
		return (1);
	if (str[i] == '\'' && (quotes_open(str, i) != 2))
		return (1);
	return (0);
}

char	*trim(char *str)
{
	char	*result;
	int		i = 0, j;
	int		len;
	int		in_single = 0, in_double = 0;

	i = 0, j = 0;
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			i++;
			continue ;
		}
		else if (str[i] == '\"' && !in_single)
		{
			in_double = !in_double;
			i++;
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	// free(str);
	return (result);
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
				head->str[i] = trim(head->str[i]);
			i++;
		}
		head = head->next;
	}
}
