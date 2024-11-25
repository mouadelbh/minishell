/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:20:34 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/11/25 18:37:51 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

char	**get_splited(char **argv, int len)
{
	char	**str;
	char	**splited;
	int		i;
	int		j;

	i = 0;
	while (argv[i])
		i++;
	str = malloc(sizeof(char *) * (i + len));
	if (!str)
		return (NULL);
	i = 0;
	splited = ft_split(argv[0], ' ');
	if (!splited)
		return (NULL);
	while (splited[i])
	{
		str[i] = ft_strdup(splited[i]);
		free(splited[i++]);
	}
	j = 1;
	while (argv[j])
		str[i++] = ft_strdup(argv[j++]);
	str[i] = NULL;
	return (free(splited), str);
}

void	split_command(t_cmd **node)
{
	t_cmd	*tmp;
	char	**new;
	int		len;

	tmp = *node;
	while (tmp)
	{
		len = count_words(tmp->argv[0], ' ');
		if (tmp->type == CMD && len > 1)
		{
			new = get_splited(tmp->argv, len);
			ft_free(tmp->argv);
			tmp->argv = new;
		}
		else if (tmp->type == CMD && tmp->argv[0][0] == 31)
			tmp->argv[0][0] = '\0';
		tmp = tmp->next;
	}
}
