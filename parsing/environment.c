/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:07 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/09 00:04:38 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static void	env_lstadd_back(t_list **head, t_list *new)
{
	t_list	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static void	set_env(t_list **head, char **envp)
{
	t_list	*new;
	int		i;

	i = 0;
	while (envp[i])
	{
		new = malloc(sizeof(t_list));
		if (!new)
			return ;
		new->content = ft_strdup(envp[i]);
		new->empty_value = 0;
		new->next = NULL;
		env_lstadd_back(head, new);
		i++;
	}
}

t_list	*get_pwd(void)
{
	t_list	*env;
	char	*tmp;

	env = malloc(sizeof(t_list));
	if (!env)
		return (NULL);
	tmp = malloc(sizeof(char) * PATH + 1);
	if (!tmp)
		return (free(env), NULL);
	if (!getcwd(tmp, PATH))
		return (free(env), free(tmp), NULL);
	env->content = ft_strjoin("PWD=", tmp);
	env->next = NULL;
	free(tmp);
	return (env);
}

t_list	*get_shlvl(void)
{
	t_list	*tmp;

	tmp = malloc(sizeof(t_list));
	if (!tmp)
		return (NULL);
	tmp->content = malloc(8);
	if (!tmp->content)
		return (free(tmp), NULL);
	tmp->content[0] = '\0';
	ft_strlcat(tmp->content, "SHLVL=1", 8);
	tmp->next = NULL;
	return (tmp);
}

void	create_env(t_list **head)
{
	env_lstadd_back(head, get_pwd());
	env_lstadd_back(head, get_shlvl());
}

void	init(t_list **data, char **env)
{
	exit_status = 0;
	exit_status = 0;
	signal(SIGINT, handlesig);
	signal(SIGQUIT, SIG_IGN);
	if (!env[0])
		create_env(data);
	else
		set_env(data, env);
	if (!data)
		return ;
}
