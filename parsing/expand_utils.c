/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 14:35:27 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/26 14:00:56 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	reset_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == -1)
		{
			str[i] = '$';
			break ;
		}
		i++;
	}
}

int	find(char *tmp, int i, t_list *env, int *size)
{
	int	j;

	j = 0;
	*size = 0;
	i++;
	while (!check_case(tmp, i + *size))
		*size += 1;
	tmp += i;
	while (env)
	{
		if (!ft_strncmp(tmp, env->content, *size) && env->content[*size] == '=')
			return (j);
		env = env->next;
		j++;
	}
	return (-1);
}

char	*fill_string(char *tmp, t_list *env, int env_len, int size)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(ft_strlen(tmp) + ft_strlen(env->content + env_len) - size + 1);
	if (!new)
		return (NULL);
	while (tmp[i] && tmp[i] != -1)
	{
		new[i] = tmp[i];
		i++;
	}
	j = i + size + 1;
	while (env->content[env_len])
		new[i++] = env->content[env_len++];
	while (tmp[j])
		new[i++] = tmp[j++];
	new[i] = '\0';
	return (new);
}
