/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:42 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/12/30 17:51:28 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "minishell.h"

# define SUCCESS 0
# define GENERAL_ERROR 1
# define PARSE_ERROR 2
# define COMMAND_INVOKED_CANNOT_EXECUTE 126
# define COMMAND_NOT_FOUND 127
# define INVALID_EXIT_ARGUMENT 128
# define EXIT_OUT_OF_RANGE 255
# define CTRL_C 130

# define PATH 4096

typedef enum e_token
{
	NONE,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	EXPORT,
	ARG,
	CMD
}	t_token;

typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;

# define BUFFER_SIZE	4098

typedef struct s_expand
{
	int		size;
	int		ca;
}	t_expand;

typedef struct s_token
{
	char			**str;
	t_token			type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_line;

void	handledoc(int sig);
void	ft_free(char **arg);
void	split_command(t_cmd **node);
void	lexer(char **arg, t_line **head);
void	tokenize_cmd(char *str, t_line *tmp);
void	tokenize(char *arg, t_line *tmp);
void	tokenize_arg(char **arg, int *i, t_line *tmp, int *flag);
void	tokenize_quotarg(char **arg, int *i, t_line *tmp);
void	init(t_list **data, char **env);
void	triming_quotes(t_line *head);
void	get_final_list(t_line **head, t_cmd **cmd);
void	free_line(t_line *head);
void	handlesig(int sig);
void	lstadd_line(t_line **head, t_line *new);
void	arange_arguments(t_line *head, t_line **final);
void	handledoc(int sig);
void	handlehang(int sig);
void	update_env(t_cmd *cmd, t_data *data);
void	reset_expand(char *str);
void	expanding(t_line **head, t_list *env);
void	unflag_spaces(char **line);
void	flag_spaces(char *line);
void	fill_arguments(char **arg, int *i, t_line *tmp, int *flag);
void	execute_signal(t_data *data, int temp_fd, char *temp_file);
int		set_exit_status(int status);
int		count_arguments(char **arg, int *i, t_line *tmp);
int		print_error(char *str);
int		parse(char *line, t_line **head, t_list *env, t_data *ex_data);
int		check_token(int c);
int		checkquotes(char *line, t_data *data);
int		checkspaces(char *line);
int		quotes_open(char *s, int i);
int		is_space(int c);
int		isredir(int i);
int		checkspaces(char *line);
int		is_empty(char *str);
int		count_words(const char *s, char c);
int		alloc_exit(char *str, int exit);
int		empty_node(t_line *node);
int		handle_empty_node(t_line **node);
int		handle_redir(t_line **node, t_cmd **new);
int		handle_pipe(t_line **node, t_cmd **new);
int		find(char *tmp, int i, t_list *env, int *size);
int		check_case(char *str, int i);
char	*find_and_replace(char *line, t_list *data, int flag);
char	*spacing(char *line);
char	*ft_itoa(int value);
char	*delete(char *tmp, int size);
char	*fill_string(char *tmp, t_list *env, int env_len, int size);
char	*expand_exit(char *str, int i, int exit);
char	*expand_string(char *line, t_list *envp);
t_list	*get_shlvl(void);
t_list	*get_pwd(void);
t_line	*copy_line_node(t_line *node);
t_token	get_token(char *str);

#endif
