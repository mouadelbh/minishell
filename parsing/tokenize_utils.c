/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 16:01:05 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 16:01:26 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	count_arguments(char **arg, int *i, t_line *tmp)
{
	int	count;
	int	j;

	j = *i;
	count = 0;
	while (arg[j] && get_token(arg[j]) == NONE)
	{
		count++;
		j++;
		if (isredir(tmp->prev->type))
			break ;
	}
	return (count);
}

void	fill_arguments(char **arg, int *i, t_line *tmp, int *flag)
{
	int	j;

	j = 0;
	while (arg[*i] && get_token(arg[*i]) == NONE && \
	!ft_strchr(arg[*i], '\'') && !ft_strchr(arg[*i], '\"'))
	{
		tmp->str[j] = ft_strdup(arg[*i]);
		j++;
		*i += 1;
		if (isredir(tmp->prev->type))
		{
			*flag = 1;
			break ;
		}
	}
	tmp->str[j] = NULL;
}
