/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spacing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:46 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/10/14 17:26:57 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
		{
			buffer[j++] = ' ';
			buffer[j++] = line[i++];
			if (!line[i])
				break ;
			if (i && ft_strchr("<>", line[i]) && line[i] == line[i - 1])
			{
				buffer[j++] = line[i++];
				buffer[j++] = ' ';
			}
			buffer[j++] = ' ';
		}
		else
			buffer[j++] = line[i++];
	}
	buffer[j] = '\0';
	str = ft_strdup(buffer);
	free(line);
	free(buffer);
	return (str);
}
