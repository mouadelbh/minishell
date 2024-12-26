NAME	=	minishell
PARS_F	=	parsing/parsing.c parsing/lexer.c parsing/arrange.c \
			parsing/tokenize.c parsing/utils.c parsing/checker.c \
			parsing/environment.c parsing/spacing.c parsing/expanding.c \
			parsing/triming.c parsing/finalparse.c parsing/free.c \
			parsing/utils2.c parsing/utils3.c parsing/utils4.c \
			parsing/utils5.c parsing/expand_utils.c parsing/tokenize_utils.c\
			parsing/finalparse_utils.c
EXEC_F	=	minishell.c builtins/echo.c builtins/cd.c \
 			builtins/pwd.c builtins/export.c builtins/unset.c \
 			builtins/env.c builtins/exit.c execution/execution.c \
			execution/pipe.c execution/commands.c \
			execution/execute.c execution/files.c execution/ios.c \
			execution/utils.c execution/init.c execution/heredoc.c \
			execution/free.c builtins/export_utils.c \
			execution/checker.c execution/free_utils.c builtins/export_utils_2.c \
			execution/utils2.c execution/utils3.c execution/heredoc_utils.c \
			execution/init_utils.c execution/files_utils.c execution/commands_utils.c \
			execution/commands_utils_2.c
FILES	=	$(PARS_F) $(EXEC_F)
HEADER	=	includes/parsing.h includes/minishell.h
OBJS	=	$(FILES:.c=.o)
LIBFT	=	libft/libft.a
FLAGS	=	-g -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(HEADER)
	@cc $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(S_NAME): $(S_OBJS) $(LIBFT)
	@cc $(S_FLAGS) -o $(S_NAME) $(S_OBJS) $(LIBFT) -lreadline

$(LIBFT):
	@make -C libft

%.o: %.c
	@cc $(FLAGS) -c $< -o $@

%.sanitize.o: %.c
	@cc $(S_FLAGS) -c $< -o $@

clean:
	@make -C libft clean
	@rm -f $(OBJS) $(S_OBJS)

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME) $(S_NAME)

re: fclean all

.PHONY: all clean fclean re
