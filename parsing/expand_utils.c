/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 14:35:27 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 16:26:28 by mel-bouh         ###   ########.fr       */
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

// int	get_env_length(t_list *env, int ca)
// {
// 	int	env_len;

// 	env_len = 0;
// 	while (ca--)
// 		env = env->next;
// 	while (env->content[env_len] && env->content[env_len] != '=')
// 		env_len++;
// 	return (env_len + 1);
// }

// char	*delete(char *tmp, int size)
// {
// 	char	*new;
// 	int		j;
// 	int		i;

// 	new = malloc(ft_strlen(tmp) + 1);
// 	if (!new)
// 		return (NULL);
// 	i = 0;
// 	while (tmp[i] && tmp[i] != -1)
// 	{
// 		new[i] = tmp[i];
// 		i++;
// 	}
// 	j = i;
// 	i = i + size + 1;
// 	while (tmp[i])
// 		new[j++] = tmp[i++];
// 	new[j] = '\0';
// 	free(tmp);
// 	return (new);
// }

// int	alloc_exit(char *str, int exit)
// {
// 	int	size;
// 	int	len;

// 	len = 0;
// 	if (exit <= 0)
// 		len = 1;
// 	while (exit)
// 	{
// 		len++;
// 		exit /= 10;
// 	}
// 	size = ft_strlen(str) + len;
// 	return (size);
// }

// char	*expand_exit(char *str, int i, int exit)
// {
// 	int		size;
// 	int		j;
// 	int		k;
// 	char	*tmp;
// 	char	*new;

// 	size = alloc_exit(str, exit);
// 	new = malloc(size + 1);
// 	if (!new)
// 		return (NULL);
// 	j = 0;
// 	k = 0;
// 	while (j < i)
// 	{
// 		new[j] = str[j];
// 		j++;
// 	}
// 	tmp = ft_itoa(exit);
// 	while (tmp[k])
// 		new[j++] = tmp[k++];
// 	i += 2;
// 	while (str[i])
// 		new[j++] = str[i++];
// 	new[j] = '\0';
// 	return (free(str), free(tmp), new);
// }
