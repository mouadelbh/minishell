/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:58:58 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/25 16:01:18 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	tokenize_quotarg(char **arg, int *i, t_line *tmp)
{
	tmp->str = malloc(sizeof(char *) * 2);
	tmp->str[0] = ft_strdup(arg[*i]);
	tmp->str[1] = NULL;
	*i += 1;
	tmp->type = ARG;
}

void	tokenize_arg(char **arg, int *i, t_line *tmp, int *flag)
{
	int	count;

	count = count_arguments(arg, i, tmp);
	tmp->str = malloc(sizeof(char *) * (count + 1));
	if (!tmp->str)
		return ;
	fill_arguments(arg, i, tmp, flag);
	tmp->type = ARG;
}

void	tokenize(char *arg, t_line *tmp)
{
	tmp->str = malloc(sizeof(char *) * 2);
	if (!tmp->str)
		return ;
	tmp->str[0] = ft_strdup(arg);
	tmp->str[1] = NULL;
	tmp->type = get_token(arg);
}

void	tokenize_cmd(char *str, t_line *tmp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	tmp->str = malloc(sizeof(char *) * 2);
	if (!tmp->str)
		return ;
	tmp->str[0] = malloc(ft_strlen(str) + 1);
	if (!tmp->str[0])
		return ;
	while (str[i])
		tmp->str[0][j++] = str[i++];
	tmp->str[0][j] = '\0';
	tmp->str[1] = NULL;
	tmp->type = CMD;
}
