/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:29 by zelbassa          #+#    #+#             */
/*   Updated: 2024/10/20 22:42:20 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_getenv(char *name, t_data *data)
{
	int 	i;
	t_list	*temp = data->envp;

	if (!name || !data || !data->envp)
		return NULL;
	i = 0;
	while (temp)
	{
		if (ft_strncmp(temp->content, name, ft_strlen(name)))
			return (ft_strdup(temp->content));
		temp = temp->next;
	}
	return (NULL);
}

int	reset_shell(t_data *data)
{
	// data->head = data->head->next;
	if (data->arg)
	{
		free(data->arg);
		data->arg = NULL;
	}
	// rl_clear_history();
	minishell(data);
	return (1);
}

int	modify_env_value(char *name, char *new_value, t_data *data)
{
	char *str;

	str = getenv(name);
	if (!str)
		perror("getenv");
	else
		set_list_var(data, name, new_value);
	return (1);
}

int	builtin(char *cmd)
{
	char	*builtin[8];
	int		i;

	i = 0;
	builtin[0] = "echo";
	builtin[1] = "cd";
	builtin[2] = "pwd";
	builtin[3] = "export";
	builtin[4] = "unset";
	builtin[5] = "env";
	builtin[6] = "exit";
	builtin[7] = NULL;
	while (i < 7)
	{
		if (ft_strncmp(builtin[i], cmd, 0) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(t_data *data, char **cmd)
{
	int	res;

	res = 0;
	if (ft_strncmp(cmd[0], "pwd", 0) == 0)
		res = ft_pwd(data, cmd);
	else if (ft_strncmp(cmd[0], "env", 0) == 0)
		res = ft_env(data, cmd);
	else if (ft_strncmp(cmd[0], "echo", 0) == 0)
		res = ft_echo(data, cmd);
	else if (ft_strncmp(cmd[0], "cd", 0) == 0)
		res = ft_cd(data, cmd);
	else if (ft_strncmp(cmd[0], "unset", 0) == 0)
		res = ft_unset(data, cmd);
	else if (ft_strncmp(cmd[0], "export", 0) == 0)
		res = ft_export(data, cmd);
	return (res);
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

void set_list_var(t_data *data, char *name, char *new_value)
{
	int 	len;
	t_list *current;
	char	*temp;

	len = ft_strlen(name);
	current = data->envp;
	while (current)
	{
		if (ft_strncmp(current->content, name, len) == 0)
		{
			free(current->content);
			current->content = NULL;
			temp = ft_strjoin(name, "=");
			current->content = ft_strjoin(temp, new_value);
			if (!current->content)
			{
				ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
				return;
			}
			break;
		}
		current = current->next;
	}
}

void	printa(char *message, char **str)
{
	int	i;

	i = 0;
	printf("%s: ", message);
	while (str[i])
	{
		printf("%s ", str[i]);
		i++;
	}
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

	if (!count)
		count = 0;
	printf("Here: %d\n", count);
	count++;
}

char	*get_full_cmd(char *av, char **env)
{
	int		i;
	// char	*result;
	char	*full_cmd;
	char	**path;

	i = 0;
	(void)env;
	path = ft_split(getenv("PATH"), ':');
	if (!path)
		perror("Path error");
	while (path[i])
	{
		full_cmd = ft_strjoin(ft_strjoin(path[i], "/"), av);
		// free(result);
		if (access(full_cmd, X_OK | F_OK) == 0)
		{
			return (full_cmd);
		}
		// free(full_cmd);
		i++;
	}
	// free_arr(path);
	return (NULL);
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


void show_cmd(t_cmd *cmd_list)
{
    int 	i = 0;
    t_cmd	*temp = cmd_list;

	while (temp)
	{
		printf("The command is: %s\n", temp->cmd);
		printf("The type is: %i\n", temp->type);
		temp = temp->next;
	}
}

char *ft_strcat(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL); // Both are NULL
	if (!s1)
		return (strdup(s2)); // s1 is NULL, return a copy of s2
	if (!s2)
		return (strdup(s1)); // s2 is NULL, return a copy of s1
	len1 = strlen(s1);
	len2 = strlen(s2);
	char *dest = malloc(len1 + len2 + 1);
	if (!dest)
		return (NULL); // Check for allocation failure
	memcpy(dest, s1, len1);
	memcpy(dest + len1, s2, len2);
	dest[len1 + len2] = '\0';
	return (dest); // Return the pointer to the new string
}

char	*to_str(char **arr)
{
	char	*result;
	int		i;

	i = 0;
	result = NULL;
	while (arr[i])
	{
		result = ft_strcat(result, arr[i]);
		if (arr[i + 1])
			result = ft_strcat(result, " ");
		i++;
	}
	return (result);
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
		else
			return (NULL);
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

void	show_command_info(t_cmd *command)
{
	t_cmd *cmd = command;
	while (cmd)
	{
		ft_putstr_fd("------------------\n", 2);
		ft_putstr_fd("Command: ", 2);
		ft_putstr_fd(cmd->cmd?cmd->cmd:"NULL", 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The fd in is: ", 2);
		ft_putnbr_fd(cmd->io_fds->in_fd, 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The file to read from: ", 2);
		ft_putstr_fd(cmd->io_fds->infile?cmd->io_fds->infile:"NULL", 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The file to write to: ", 2);
		ft_putstr_fd(cmd->io_fds->outfile?cmd->io_fds->outfile:"NULL", 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("The fd out is: ", 2);
		ft_putnbr_fd(cmd->io_fds->out_fd, 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("Type: ", 2);
		ft_putnbr_fd(cmd->type, 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("Is piped? ", 2);
		ft_putstr_fd(cmd->pipe_output?"Yes":"No", 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("pipe[0]: ", 2);
		ft_putnbr_fd(cmd->pipe_fd[0], 2);
		ft_putchar_fd('\n', 2);
		ft_putstr_fd("pipe[1]: ", 2);
		ft_putnbr_fd(cmd->pipe_fd[1], 2);
		ft_putchar_fd('\n', 2);
		cmd = cmd->next;
	}
}

void	show_command_ios(t_cmd *cmd)
{
	t_cmd	*temp = cmd;
	while (temp)
	{
		show_io_fds(temp->io_fds);
		temp = temp->next;
	}
}

void	show_io_fds(t_io_fds *io_fds)
{
	if (!io_fds)
	{
		ft_putstr_fd("io_fds is NULL\n", 2);
		return;
	}
	ft_putstr_fd("in_fd: ", 2);
	ft_putnbr_fd(io_fds->in_fd, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("out_fd: ", 2);
	ft_putnbr_fd(io_fds->out_fd, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("infile: ", 2);
	ft_putstr_fd(io_fds->infile?io_fds->infile:"Null infile", 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("outfile: ", 2);
	ft_putstr_fd(io_fds->outfile?io_fds->outfile:"Null outfile", 2);
	ft_putchar_fd('\n', 2);
}

void set_cmd_strings(t_cmd *cmd)
{
	t_cmd	*current = cmd;
	int		i;
	size_t	total_length;

	while (current != NULL)
	{
		total_length = 0;
		i = 0;
		while (current->argv[i] != NULL)
		{
			total_length += ft_strlen(current->argv[i]) + 1;
			i++;
		}
		current->cmd = malloc(total_length * sizeof(char));
		if (current->cmd == NULL)
		{
			perror("Failed to allocate memory");
			exit(EXIT_FAILURE);
		}
		current->cmd[0] = '\0';
		i = 0;
		while (current->argv[i] != NULL)
		{
			strcat(current->cmd, current->argv[i]);
			if (current->argv[i + 1] != NULL)
				strcat(current->cmd, " ");
			i++;
		}
		current = current->next;
	}
}

char	**set_list_arra(t_list *env)
{
	char	**result;
	t_list	*temp = env;

	int i = ft_lstsize(env);
	result = malloc(sizeof(char *) * (i + 1));
	if (!result)
		return NULL;
	result[i] = NULL;
	i = 0;
	while (temp)
	{
		result[i] = temp->content;
		temp = temp->next;
		i++;
	}
	return (result);
}