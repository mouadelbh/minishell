/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/12/13 21:20:18 by mel-bouh         ###   ########.fr       */
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

static int	check_case(char *str, int i)
{
	if (!str[i] || is_space(str[i]) || str[i] == '=' \
	|| str[i] == '\'' || str[i] == '\"' || str[i] == '/' || str[i] == '.')
		return (1);
	if (check_token(str[i]))
		return (1);
	return (0);
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

char	*replace(char *tmp, int k, t_list *env, int size, int flag)
{
	char	*new;
	int		i;
	int		j;
	int		env_len;

	i = 0;
	env_len = 0;
	while (k--)
		env = env->next;
	while (env->content[env_len] && env->content[env_len] != '=')
		env_len++;
	env_len++;
	if (flag == 1 && count_words(env->content + env_len, ' ') > 1)
	{
		reset_expand(tmp);
		return (tmp);
	}
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
	free(tmp);
	return (new);
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
	free(tmp);
	return (new);
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
	int		i;
	int		size;
	int		ca;
	int		j;
	int		c;
	char	*tmp;

	if (!str)
		return (NULL);
	tmp = ft_strdup(str);
	i = 0;
	while (str[i])
	{
		j = 0;
		c = 0;
		if (str[i] == -1 && str[i + 1] == '?')
			tmp = expand_exit(tmp, i, exit_status);
		else if (str[i] == -1)
		{
			ca = find(str, i, env, &size);
			if (ca >= 0)
				tmp = replace(tmp, ca, env, size, flag);
			else if (ca < 0 && flag == 0)
				tmp = delete(tmp, size);
			else
				reset_expand(tmp);
		}
		if (tmp == NULL)
			return (free(str), NULL);
		i++;
	}
	free(str);
	return (tmp);
}
