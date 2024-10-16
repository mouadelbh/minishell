/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:18:31 by prizmo            #+#    #+#             */
/*   Updated: 2024/10/16 15:05:52 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exit_status = 0;
char	*string = NULL;

void	free_cmd(t_cmd **head)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	tmp = (*head);
	while (*head)
	{
		i = 0;
		while ((*head)->argv[i])
			free((*head)->argv[i++]);
		free((*head)->argv);
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}
}

// void	handlesig(int signal)
// {
// 	if (signal == SIGINT)
// 	{
// 		printf("ctrc\n");
// 		printf("\n");
// 		// if (string)
// 		// 	free(string);
// 		exit_status = 130;
// 	}
// 	else if (signal == EOF)
// 	{
// 		printf("EOF\n");
// 		if (string)
// 			free(string);
// 		exit_status = -10;
// 	}
// }

int	main(int ac, char **av, char **env)
{
	(void)av;
	struct sigaction sa;
	t_parse	data;
	t_line	*head;
	t_cmd	*cmd;

	if (ac != 1)
		return (printf("minishell doesn't take any arguments\n"), 1);
	head = NULL;
	sa.sa_handler = handlesig;
	sa.sa_flags = 0;
	cmd = NULL;
	data.exit = 0;
	data.env = NULL;
	init(&data.env, env);
	while (1)
	{
		string = readline("\x1b[36mminishell->\x1b[0m ");
		if (ft_strlen(string) > 0)
		{
			add_history(string);
			exit_status = parse(string, &head, env, &data);
			// if (sigaction(SIGINT, &sa, NULL) == -1)
			// {
			// 	perror("SIGINT error\n");
			// 	return (1);
			// }
			// if (sigaction(EOF, &sa, NULL) == -1)
			// {
			// 	perror("EOF error\n");
			// 	return (1);
			// }
			if ((!exit_status || exit_status == 130) && head)
				get_final_list(&head, &cmd);
			// while (head)
			// {
			// 	printf("  this is a node\n");
			// 	printf("------------------\n");
			// 	for (int i = 0;head->str[i];i++)
			// 		printf("str: %s\n", head->str[i]);
			// 	printf("type: %i\n", head->type);
			// 	head = head->next;
			// }
			free_line(&head);
			free_cmd(&cmd);
		}
	}
	return (0);
}
