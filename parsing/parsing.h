/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:56:42 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/10/14 17:50:47 by mel-bouh         ###   ########.fr       */
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
# include "../minishell.h"

# define PATH_MAX 4096
typedef enum
{
	NONE,		// 0
	PIPE,		// 1
	REDIR_IN,	// 2
	REDIR_OUT,	// 3
	APPEND,		// 4
	HEREDOC,	// 5
	EXPORT,		// 6
	ARG,		// 7
	CMD			// 8
}	t_token;

#define BUFFER_SIZE	4098
extern char	*string;

typedef struct	s_cmd
{
	char			**argv;
	char			*input_file;
	char			*output_file;
	int				type;
	int				*pipe_fd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct	s_env
{
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct	s_parsing_data
{
	int		exit;
	t_env	*env;
}	t_parse;

typedef struct	s_token
{
	char 			**str;
	t_token			type;
	t_parse			*data;
	struct s_token	*next;
}	t_line;

void	ft_free(char **arg);
void	lexer(char **arg, t_line **head, t_parse *data);
void	tokenize_cmd(char *str, t_line *tmp);
void	tokenize(char *arg, t_line *tmp);
void	tokenize_arg(char **arg, int *i, t_line *tmp);
void	tokenize_quotarg(char **arg, int *i, t_line *tmp, char c);
void	init(t_env **data, char **env);
void	triming_quotes(t_line *head);
void	get_final_list(t_line **head, t_cmd **cmd);
void	free_line(t_line **head);
int		parse(char *line, t_line **head, char **env, t_parse *data);
int		check_token(int c);
int		special_char(char *str, int i);
int		checkquotes(char *line);
int		checkspaces(char *line);
int		quotes_open(char *s, int i);
int		is_space(char c);
int		checkspaces(char *line);
int		checkquotes(char *line);
int		check_case_1(char c);
char	*find_and_replace(char *line, t_parse *data);
char	*spacing(char *line);
t_token	get_token(char *str);

#endif
