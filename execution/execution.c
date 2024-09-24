/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 09:35:10 by prizmo            #+#    #+#             */
/*   Updated: 2024/09/24 16:11:17 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	reset_shell(t_data *data)
{
	// data->head = data->head->next;
	if (data->arg)
	{
		// free(data->arg);
		data->arg = NULL;
	}
	// rl_clear_history();
	// minishell(data);
	return (1);
}

int	ft_error(int error, t_data *data)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error == 0)
		ft_putstr_fd("syntax error: cannot find a closing quote\n",
			STDERR_FILENO);
	else if (error == 1)
		ft_putstr_fd("error when forking\n",
			STDERR_FILENO);
	else if (error == 2)
		ft_putstr_fd("error finding command\n", STDERR_FILENO);
	else if (error == 3)
		ft_putstr_fd("Could not find corresponding path\n", STDERR_FILENO);
	reset_shell(data);
	return (EXIT_FAILURE);
}

void	printa(char *message, char **str)
{
	printf("%s: ", message);
	for (int i = 0; str[i]; i++)
		printf("%s\n", str[i]);
}

int	count_pipes(t_data *data)
{
	int	i;
	int	k;
	t_line	*temp;

	i = 0;
	k = 0;
	temp = data->head;
	while (temp)
	{
		while (temp->str[i])
		{
			if (temp->type == 1)
				k++;
			i++;
		}
		temp = temp->next;
	}
	// free(temp);
	return (k);
}

void	debug()
{
	static int count;
	
	count += 1;
	printf("Here: %d\n", count);
}

static char	*get_full_cmd(char *av, char **env)
{
	int		i;
	char	*result;
	char	*full_cmd;
	char	**path;

	i = 0;
	path = ft_split(getenv("PATH"), ':');
	if (!path)
		perror("Path error");
	while (path[i])
	{
		// result = ft_strjoin(path[i], "/");
		full_cmd = ft_strjoin(ft_strjoin(path[i], "/"), av);
		// free(result);
		if (access(full_cmd, X_OK | F_OK) == 0)
			return (full_cmd);
		// free(full_cmd);
		i++;
	}
	// free_arr(path);
	return (NULL);
}

void	exec_cmd(char *av, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(av, ' ');
	if (cmd[0][0] == '/')
		path = ft_strdup(cmd[0]);
	else
		path = get_full_cmd(cmd[0], env);
	if (!path)
	{
		// cmd = NULL;
		// free_arr(cmd);
		perror(cmd[0]);
		// exit(1);
	}
	// printf("here ;");
	if (execve(path, cmd, env) == -1)
	{
		// cmd = NULL;
		// free_arr(cmd);
		// perror("execve");
		return ;
	}
}

char	*array_to_string(t_line *temp)
{
	size_t total_length = 0;
	char *cmd;
	size_t cmd_size;
	t_line *current = temp;

	// Calculate the total length needed
	while (current && current->str[0][0] != '|' && current->str[0][0] != '>' && current->str[0][0] != '<')
	{
		for (size_t i = 0; current->str[i]; i++)
			total_length += strlen(current->str[i]) + 1;
		current = current->next;
	}

	// Allocate memory for the result string
	cmd_size = total_length + 1; // +1 for the null terminator
	cmd = (char *)malloc(cmd_size);
	if (!cmd)
	{
		perror("malloc");
		return NULL;
	}
	cmd[0] = '\0'; // Initialize the result string

	// Concatenate each element with a space
	current = temp;
	while (current && current->str[0][0] != '|' && current->str[0][0] != '>' && current->str[0][0] != '<')
	{
		for (size_t i = 0; current->str[i]; i++)
		{
			ft_strlcat(cmd, current->str[i], cmd_size);
			if (current->str[i + 1] || (current->next && 
				(current->next->type == 7 || current->next->type == 8)))
				ft_strlcat(cmd, " ", cmd_size);
		}
		current = current->next;
	}
	return cmd;
}

int	count_symbols(t_data *data)
{
	int		i;
	t_line	*temp = data->head;

	i = 0;
	while (temp)
	{
		if (temp->type >= 1 && temp->type <= 5)
			i++;
		temp = temp->next;
	}
	return (i);
}

char	*new_strjoin(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
	{
		if (s1)
			return (ft_strdup(s1));
		if (s2)
			return (ft_strdup(s2));
	}
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	while (*s1)
		*result++ = *s1++;
	*result = ' ';
	result++;
	while (*s2)
		*result++ = *s2++;
	*result = '\0';
	return (result);
}

t_cmd *create_node(char *cmd, int type)
{
	t_cmd *node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	node->str = ft_strdup(cmd);
	node->type = type;
	node->priority = 0;
	node->next = NULL;
	return (node);
}

int	single_command(t_data *data, char *cmd)
{
	t_line	*temp = data->head;
	int pid;

	while (temp)
	{
		if (temp->next && temp->next->type == 7)
			temp = temp->next;
		pid = fork();
		if (pid == -1)
			return (ft_error(1, data));
		if (pid == 0)
			exec_cmd(cmd, data->envp);
		waitpid(0, NULL, 0);
		temp = temp->next;
	}
	return (0);
}

char	*ft_strcat(char *s1, char *s2)
{
	char	*dest;
	int		i;

	i = 0;
	dest = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	while (s1 && *s1)
		*dest++ = *s1++;
	while(s2 && *s2)
		*dest++ = *s2++;
	*dest = '\0';
	return (dest);
}

// t_cmd	*fill_tree(t_line *temp)
// {
// 	t_cmd	*root = NULL;
// 	t_cmd	*current = NULL;
// 	t_cmd	*node;
// 	char *cmd = NULL;

// 	root = create_node(temp->str[0], temp->type);
// 	while (temp)
// 	{
// 		// while (temp->type == 7 || temp->type == 8)
// 		// {
// 		// 	cmd = ft_strcat(cmd, temp->str[0]);
// 		// 	temp = temp->next;
// 		// }
// 		node = create_node(cmd?cmd:temp->str[0], temp->type);
// 		current = set_node(node, root);
// 		if (!root)
// 			root = current;
// 		temp = temp->next;
// 	}
// 	return (root);
// }

t_cmd	fill_tree(t_line *data)
{
	t_cmd	*result;
	
	result = (t_cmd *)malloc(sizeof(t_cmd));
	result->str = data->str[0];
	result->type = data->type;
	result->next = data->next;
	return (result);
}

t_cmd *build_execution_tree(t_line *temp)
{
	t_cmd	*node;

	node = fill_tree(temp);
	return (node);
}

// void execute_tree(t_cmd *root, char **envp)
// {
// 	int	pipefd[2];
// 	int	pid;

// 	debug();
// 	printf("Type: %d\n", root->type);
// 	if (!root)
// 		return ;
// 	if (root->type == 1)
// 	{
// 		pipe(pipefd);
// 		if ((pid == fork()) == 0)
// 		{
// 			close(pipefd[0]);
// 			dup2(pipefd[1], STDOUT_FILENO);
// 			close(pipefd[1]);
// 			execute_tree(root->next, envp);
// 			exit(EXIT_SUCCESS);
// 		}
// 	}
// 	else
// 	{
// 		if ((pid == fork()) == 0)
// 			execute_command(root->str, envp);
// 		else
// 			waitpid(pid, NULL, 0);
// 	}
// }

int	complex_command(t_data *data, char *cmd, int symbol)
{
	t_cmd	*command;
	t_line	*temp = data->head;

	command = build_execution_tree(temp);
	if (command)
		printf("Command\n");
	else
		printf("Command not set :,(");
	execute_tree(command, data->envp);
	return (0);
}

int	handle_input(t_data *data)
{
	t_line	*temp = data->head;
	char	*cmd;

	int i = count_symbols(data);
	cmd = array_to_string(temp);
	if (i == 0)
		single_command(data, cmd);
	else
		complex_command(data, cmd, i);
	return (0);
}

int	minishell(t_data *data)
{
	while (1)
	{
		data->head = NULL;
		data->arg = readline(READLINE_MSG);
		if (data->arg == NULL || data->arg[0] == '\0')
			return (reset_shell(data));
		add_history(data->arg);
		parse(data->arg, &data->head, data->envp);
		handle_input(data);
		reset_shell(data);
		if (data->status == 0)
			break ;
	}
	return (1);
}
