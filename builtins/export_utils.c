/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:34:12 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/26 13:59:59 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_occurance(char *str, char *oc)
{
	int		i;
	int		j;
	char	*result;

	if (!str || !oc)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (perror("Failed to allocate memory"), NULL);
	i = 0;
	while (str[i])
	{
		j = 0;
		while (oc[j] && str[i + j] == oc[j])
			j++;
		if (!oc[j])
		{
			strncpy(result, str, i);
			result[i] = '\0';
			return (result);
		}
		i++;
	}
	return (free(result), NULL);
}

char	*new_substr(const char *str, int c)
{
	int		i;
	int		j;
	char	*result;

	j = 0;
	i = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != c)
		i++;
	result = malloc(sizeof(char) * (i + 1));
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[i] = '\0';
	return (result);
}

char	*get_key(char *value, char **env_value)
{
	char	*key;
	int		i;

	i = is_valid_env_name(value);
	if (i == 1)
	{
		key = find_occurance(value, "=");
		*env_value = ft_substr(value, (ft_strlen(key) + 1), ft_strlen(value));
	}
	else
		*env_value = NULL;
	return (key);
}

int	append_env_value(char *key, char *env_value, t_list *envp)
{
	char	*temp;
	char	*temp2;

	while (envp)
	{
		if (ft_strncmp(key, envp->content, ft_strlen(key)) == 0)
		{
			temp2 = ft_strjoin(key, "=");
			temp = ft_strjoin(temp2, env_value);
			envp->empty_value = 0;
			free(envp->content);
			envp->content = ft_strdup(temp);
			free(temp);
			return (free(temp2), 1);
		}
		envp = envp->next;
	}
	return (0);
}
