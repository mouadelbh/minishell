/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 16:26:16 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/26 14:00:13 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

char	*replace(char *tmp, t_list *env, t_expand ex, int flag)
{
	char	*new;
	int		env_len;

	env_len = 0;
	while (ex.ca--)
		env = env->next;
	while (env->content[env_len] && env->content[env_len] != '=')
		env_len++;
	env_len++;
	if (flag == 1 && count_words(env->content + env_len, ' ') > 1)
	{
		reset_expand(tmp);
		return (tmp);
	}
	new = fill_string(tmp, env, env_len, ex.size);
	return (free(tmp), new);
}

char	*delete(char *tmp, int size)
{
	char	*new;
	int		j;
	int		i;

	new = malloc(ft_strlen(tmp) - size + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (tmp[i] && tmp[i] != -1)
	{
		new[i] = tmp[i];
		i++;
	}
	j = i;
	i = i + size + 1;
	while (tmp[i])
		new[j++] = tmp[i++];
	new[j] = '\0';
	return (free(tmp), new);
}

int	alloc_exit(char *str, int exit)
{
	int	size;
	int	len;

	len = 0;
	if (exit <= 0)
		len = 1;
	while (exit)
	{
		len++;
		exit /= 10;
	}
	size = ft_strlen(str) + len;
	return (size);
}

char	*expand_exit(char *str, int i, int exit)
{
	int		size;
	int		j;
	int		k;
	char	*tmp;
	char	*new;

	size = alloc_exit(str, exit);
	new = malloc(size + 1);
	if (!new)
		return (NULL);
	j = 0;
	k = 0;
	while (j < i)
	{
		new[j] = str[j];
		j++;
	}
	tmp = ft_itoa(exit);
	while (tmp[k])
		new[j++] = tmp[k++];
	i += 2;
	while (str[i])
		new[j++] = str[i++];
	new[j] = '\0';
	return (free(str), free(tmp), new);
}

char	*find_and_replace(char *str, t_list *env, int flag)
{
	int			i;
	t_expand	ex;
	char		*tmp;

	tmp = ft_strdup(str);
	i = 0;
	while (tmp && str[i])
	{
		if (str[i] == -1 && str[i + 1] == '?')
			tmp = expand_exit(tmp, i, g_exit_status);
		else if (str[i] == -1)
		{
			ex.ca = find(str, i, env, &ex.size);
			if (ex.ca >= 0)
				tmp = replace(tmp, env, ex, flag);
			else if (flag == 0)
				tmp = delete(tmp, ex.size);
			else
				reset_expand(tmp);
		}
		i++;
	}
	return (free(str), tmp);
}
