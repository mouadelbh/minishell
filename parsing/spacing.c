/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spacing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:46 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 15:33:13 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static void	token_spacing(char *line, char *buffer, int *i, int *j)
{
	buffer[(*j)++] = ' ';
	buffer[(*j)++] = line[(*i)++];
	if (!line[*i])
		return ;
	if (*i && ft_strchr("<>", line[*i]) && line[*i] == line[*i - 1])
	{
		buffer[(*j)++] = line[(*i)++];
		buffer[(*j)++] = ' ';
	}
	buffer[(*j)++] = ' ';
}

char	*spacing(char *line)
{
	int		i;
	int		j;
	char	*buffer;
	char	*str;

	i = 0;
	j = 0;
	buffer = malloc(ft_strlen(line) * 3 + 1);
	if (!buffer)
		return (NULL);
	while (line[i])
	{
		if (check_token(line[i]) && !quotes_open(line, i))
			token_spacing(line, buffer, &i, &j);
		else
			buffer[j++] = line[i++];
	}
	buffer[j] = '\0';
	str = ft_strdup(buffer);
	return (free(buffer), free(line), str);
}
