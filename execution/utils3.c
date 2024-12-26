/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:09:26 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/26 15:44:33 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*to_str(char **arr)
{
	char	*result;
	char	*temp;
	int		i;

	i = 0;
	result = NULL;
	while (arr[i])
	{
		temp = ft_strjoin(result, arr[i]);
		free(result);
		if (arr[i + 1])
			result = ft_strjoin(temp, " ");
		else
			result = ft_strjoin(temp, "");
		free(temp);
		i++;
	}
	return (result);
}

char	**set_list_arra(t_list *env)
{
	char	**result;
	t_list	*temp;
	int		i;

	temp = env;
	i = ft_lstsize(env);
	result = malloc(sizeof(char *) * (i + 1));
	if (!result)
		return (NULL);
	result[i] = NULL;
	i = 0;
	while (temp)
	{
		result[i] = ft_strdup(temp->content);
		if (!result[i])
		{
			while (i > 0)
				free(result[--i]);
			free(result);
			return (NULL);
		}
		temp = temp->next;
		i++;
	}
	return (result);
}
