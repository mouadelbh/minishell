/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:50:28 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/26 18:28:31 by zelbassa         ###   ########.fr       */
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

#ifndef MAX_PATH
# define MAX_PATH 4096
#endif

# define PROMPT "\001\e[45m\002>>> \001\e[0m\e[33m\002 Minishell>$ \001\e[0m\002"
extern int	g_exit_status;

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
	int				file_error;
	int				type;
	int				pipe_fd[2];
	int				pipe_output;
	t_io_fds		*io_fds;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

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
	int			exit;
	int			pipe_count;
	int			sym_count;
	int			cmd_count;
}				t_data;

void		free_io(t_cmd *cmd);
void		free_arr(char **arr);
void		free_cmd_struct(t_cmd *cmd);
int			minishell(t_data *data);
void		handle_child_term(int status);
int			ft_pwd(char **cmd);
int			ft_env(t_data *data, char **cmd, int export);
int			ft_echo(char **cmd);
int			ft_cd(t_data *data, char **cmd);
int			ft_exit(t_data *data, char **cmd);
int			ft_export(t_data *data, char **cmd);
int			ft_unset(t_data *data, char **cmd);
int			modify_env_value(char *name, char *new_value, t_data *data);
char		*ft_getenv(char *name, t_data *data);
char		*find_value(char *name, t_list *envp);
char		*new_substr(const char *str, int c);
void		create_env_value(t_data *data, char *key, int empty_value);
void		set_list_var(t_data *data, char *name, char *new_value);
void 		reset_shell(t_data *data, int i);
void		free_env(t_list **envp);
void		free_env(t_list **envp);
int			builtin(char *cmd);
int 		exec_builtin(t_data *data, char **cmd);
int 		count_pipes(t_data *data);
int			count_symbols(t_data *data);
char		*ft_strcat(char *dest, char *src);
char 		*get_full_cmd(char *av, char **env);
char 		*to_str(char **arr);
char 		**set_list_arra(t_list *envp);
void 		set_cmd_strings(t_cmd *cmd);
int			create_files(t_cmd *cmd, t_data *data);
int			init_command(t_cmd *cmd, t_data *data);
int			command_is_valid(t_data *data, t_cmd *cmd, int is_builtin);
int			check_permission(char *path, t_data *data);
int			command_is_valid(t_data *data, t_cmd *cmd, int is_builtin);
int			check_cmd(char *cmd);
int			check_permission(char *path, t_data *data);
int			handle_input(t_data *data);
int			handle_execute(t_data *data);
int			exec_cmd(char **command, char **envp, t_data *data);
int			single_command(t_data *data);
int			complex_command(t_data *data);
int			should_pipe(t_cmd *cmd);
int			should_pipe(t_cmd *cmd);
int			set_values(t_data *data);
int			execute_command(t_data *data, t_cmd *cmd);
int			close_file(t_data *data, t_cmd *cmd);
void		init_cmd(t_cmd *cmd);
void		init_io(t_io_fds **io_fds);
int			init_write_to(t_cmd *cmd);
int			init_read_from(t_cmd *cmd);
int			init_append(t_cmd *cmd);
int			check_file_refs(t_cmd *cmd);
void		file_error(t_cmd *cmd, char *str);
void		close_fds(t_cmd *cmds, bool close_backups);
bool		check_infile_outfile(t_io_fds *io);
bool		remove_old_file_ref(t_io_fds *io, bool infile);
bool		redirect_io(t_io_fds *io);
bool		create_pipes(t_data *data);
void		lstadd_cmd(t_cmd **head, t_cmd *new);
int			fork_and_exec(t_data *data);
t_io_fds	*dup_io(t_io_fds *io);
bool		set_pipe_fds(t_cmd *cmds, t_cmd *c);
bool		restore_io(t_io_fds *io);
void		close_pipe_fds(t_cmd *cmds);
int			init_heredoc(t_cmd *cmd, t_data *data);
void		free_all(t_data *data, int i);
void		free_all(t_data *data, int i);
void		free_data(t_data *data, int exit_code);
int			is_valid_env_name(char *var);
void		handle_write_to(t_cmd *cmd);
void		handle_read_from(t_cmd *cmd);
void		handle_append(t_cmd *cmd);
char		*get_key(char *value, char **env_value);
int			append_env_value(char *key, char *env_value, t_list *envp);
void		free_cmd_node(t_cmd *cmd);

#endif
