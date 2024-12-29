/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:45:45 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/25 14:39:55 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	empty_node(t_line *node)
{
	int	i;

	i = 0;
	if (!node->str)
		return (1);
	while (node->str[i] && !node->str[i][0])
		i++;
	if (!node->str[i])
		return (1);
	return (0);
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

t_line	*copy_line_node(t_line *node)
{
	t_line	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(t_line));
	while (node->str[i])
		i++;
	new->str = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (node->str[i])
	{
		new->str[i] = ft_strdup(node->str[i]);
		i++;
	}
	new->str[i] = NULL;
	new->type = node->type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
