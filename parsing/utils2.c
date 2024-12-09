/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:20:34 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/09 01:01:37 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	nb_len(int value)
{
	int	len;

	len = 0;
	if (value == 0)
		return (1);
	while (value)
	{
		len++;
		value /= 10;
	}
	return (len);
}

char	*ft_itoa(int value)
{
	char	*num;
	int		len;

	len = nb_len(value);
	num = malloc(sizeof(char) * (len + 1));
	if (!num)
		return (NULL);
	num[len--] = '\0';
	if (value == 0)
	{
		num[len] = '0';
		return (num);
	}
	while (value)
	{
		num[len--] = value % 10 + '0';
		value /= 10;
	}
	return (num);
}

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
