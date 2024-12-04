/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:29:00 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/04 12:26:23 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**check_n_option(char **str, int *new_line)
{
	int	i;

	*new_line = 1;
	while (*str && ft_strncmp(*str, "-n", 2) == 0)
	{
		i = 2;
		while ((*str)[i] == 'n')
			i++;
		if ((*str)[i] == '\0')
		{
			*new_line = 0;
			str++;
		}
		else
			break ;
	}
	return (str);
}

static void	print_arguments(char **str)
{
	while (*str)
	{
		printf("%s", *str);
		if (*(str + 1))
			printf(" ");
		str++;
	}
}

int	ft_echo(t_data *data, char **str)
{
	int	new_line;

	str++;
	str = check_n_option(str, &new_line);
	print_arguments(str);
	if (new_line)
		printf("\n");
	return (0);
}