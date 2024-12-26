/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 14:56:20 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/26 14:01:24 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	check_case(char *str, int i)
{
	if (!str[i] || is_space(str[i]) || str[i] == '\'' || \
	str[i] == '\"' || str[i] == '$' || str[i] == '.' || str[i] == '/' \
	|| str[i] == '+')
		return (1);
	if (check_token(str[i]))
		return (1);
	return (0);
}

int	print_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
	g_exit_status = 2;
	return (-1);
}

void	flag_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]) && quotes_open(line, i))
			line[i] = 31;
		i++;
	}
}

void	unflag_spaces(char **line)
{
	int	i;
	int	j;

	i = 0;
	if (!line)
		return ;
	while (line[i])
	{
		j = 0;
		while (line[i][j])
		{
			if (line[i][j] == 31)
				line[i][j] = ' ';
			j++;
		}
		i++;
	}
}

// int	find(char *tmp, int i, t_list *env, int *size)
// {
// 	int	j;

// 	j = 0;
// 	*size = 0;
// 	i++;
// 	while (!check_case(tmp, i + *size))
// 		*size += 1;
// 	tmp += i;
// 	while (env)
// 	{
// 		if (!ft_strncmp(tmp, env->content, *size) && env->content[*size] == '=')
// 			return (j);
// 		env = env->next;
// 		j++;
// 	}
// 	return (-1);
// }
