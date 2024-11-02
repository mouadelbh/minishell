/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@1337.student.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:50:28 by prizmo            #+#    #+#             */
/*   Updated: 2024/11/02 20:03:14 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>      // printf, perror
# include <stdlib.h>     // malloc, free, exit
# include <unistd.h>     // read, write, access, open, close, fork, execve, getcwd, chdir, unlink, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <fcntl.h>      // open, O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TRUNC
# include <string.h>     // strerror
# include <sys/types.h>  // fork, wait, waitpid, wait3, wait4, stat, lstat, fstat, kill, opendir, readdir, closedir
# include <sys/stat.h>   // stat, lstat, fstat, open
# include <sys/wait.h>   // wait, waitpid, wait3, wait4
# include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill
# include <dirent.h>     // opendir, readdir, closedir
# include <termios.h>    // tcsetattr, tcgetattr, ioctl
# include <curses.h>     // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <term.h>       // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <errno.h>      // errno
# include <sys/ioctl.h>  // ioctl
# include <readline/readline.h>    // readline, add_history
# include <readline/history.h>     // rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay
# include "parsing.h"
# include "builtins.h"
# include "styles.h"

# define MAX_PATH 4096

extern int	exit_status;

typedef struct s_token t_line;

typedef	struct	s_io_fds
{
	int		in_fd;
	int		out_fd;
	char	*infile;
	char	*outfile;
	char	*heredoc_name;
	int		stdin_backup;
	int		stdout_backup;
}			t_io_fds;

typedef struct s_cmd {
	char			**argv;
	char			*cmd;
	int				type;
	int				*pipe_fd;
	int				pipe_output;
	t_io_fds		*io_fds;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

typedef struct s_redir_info
{
	t_cmd	*last_out;
	t_cmd	*last_in;
	t_cmd	*last_append;
	t_cmd	*last_heredoc;
} 			t_redir_info;

typedef struct s_builtin
{
	char		*command;
	int			has_args;
	char		*argument;
}				t_builtin;

typedef struct s_data
{
	pid_t		pid;
	t_line		*head;
	char		*arg;
	int			status;
	t_list		*envp;
	t_cmd		*cmd;
	char		**envp_arr;
	char		*curr_dir;
	char		*old_dir;
	int			pipe_count;
}				t_data;

void	show_command_ios(t_cmd *cmd);
char	*ft_strcat(char *dest, char *src);
int		minishell(t_data *data);
void	free_arr(char **arr);
int		ft_pwd(t_data *data, char **cmd);
int		ft_env(t_data *data, char **cmd);
int		ft_echo(t_data *data, char **cmd);
int		ft_cd(t_data *data, char **cmd);
int		ft_exit(t_data *data, char **cmd);
int		ft_export(t_data *data, char **cmd);
int		ft_unset(t_data *data, char **cmd);
int		modify_env_value(char *name, char *new_value, t_data *data);
char	*ft_getenv(char *name, t_data *data);
void	create_env_value(t_data *data, char *key);
void	set_list_var(t_data *data, char *name, char *new_value);
char	*find_value(char *name, t_list *envp);
char	*new_substr(const char *str, int c);
void	printa(char *str, char **arr);
int 	reset_shell(t_data *data);
int		builtin(char *cmd);
int 	exec_builtin(t_data *data, char **cmd);
int 	ft_error(int error, t_data *data);
int 	count_pipes(t_data *data);
void 	debug(void);
char 	*get_full_cmd(char *av, char **env);
char 	*array_to_string(t_line *head);
int		count_symbols(t_data *data);
char 	*to_str(char **arr);
char 	*new_strjoin(char *s1, char *s2);
void 	show_command_info(t_cmd *cmd_list);
void 	show_io_fds(t_io_fds *io_fds);
void 	set_cmd_strings(t_cmd *cmd);
char 	**set_list_arra(t_list *envp);
int		handle_input(t_data *data);
void	create_files(t_cmd *cmd, t_data *data);
void	init_command(t_cmd *cmd, t_data *data);
int		handle_execute(t_data *data);
int		exec_cmd(char *av, char **env, t_data *data);
int		single_command(t_data *data, char *cmd);
void	complex_command(t_data *data);
int		set_values(t_data *data);
int		execute_cmds(t_cmd *cmd_list, char **envp, t_data *data);
int		execute_command(t_data *data, t_cmd *cmd);
void	create_files(t_cmd *cmd, t_data *data);
bool	check_infile_outfile(t_io_fds *io);
int		close_file(t_data *data);
bool	remove_old_file_ref(t_io_fds *io, bool infile);
void	init_cmd(t_cmd *cmd);
void	init_io(t_io_fds **io_fds);
void	init_write_to(t_cmd *cmd, t_data *data);
void	init_read_from(t_cmd *cmd, t_data *data);
void	init_append(t_cmd *cmd, t_data *data);
void	close_fds(t_cmd *cmds, bool close_backups);
bool	redirect_io(t_io_fds *io);
bool	create_pipes(t_data *data);
void	close_pipe_fds(t_cmd *cmds, t_cmd *skip_cmd);
bool	set_pipe_fds(t_cmd *cmds, t_cmd *c);
void	init_heredoc(t_cmd *cmd, t_data *data);
bool	restore_io(t_io_fds *io);
void	valid_pipe_redirection(t_cmd *cmd);

#endif
