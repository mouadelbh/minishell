/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:34:12 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/04 13:47:51 by zelbassa         ###   ########.fr       */
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
	result = malloc(strlen(str) + 1);
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

char	*get_key(char *value, t_list *envp, char **env_value, int *append)
{
	char	*key;
	int		i;

	i = is_valid_env_name(value);
	if (i == 1)
	{
		key = find_occurance(value, "=");
		*env_value = ft_substr(value, (ft_strlen(key) + 1), ft_strlen(value));
	}
	else if (i == 2)
	{
		key = find_occurance(value, "+=");
		*env_value = ft_substr(value, (ft_strlen(key) + 2), ft_strlen(value));
		*append = 1;
	}
	else
		*env_value = NULL;
	return (key);
}

int	append_env_value(char *key, char *env_value, char *cmd, t_list *envp)
{
	int		i;
	char	*temp;

	while (envp)
	{
		if (ft_strncmp(key, envp->content, ft_strlen(key)) == 0)
		{
			temp = ft_strjoin(envp->content, env_value);
			free(envp->content);
			envp->content = temp;
			return (1);
		}
		envp = envp->next;
	}
	return (0);
}
