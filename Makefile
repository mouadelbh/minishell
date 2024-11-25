NAME	=	minishell
PARS_F	=	parsing/parsing.c parsing/lexer.c parsing/arrange.c \
			parsing/tokenize.c parsing/utils.c parsing/checker.c \
			parsing/environment.c parsing/spacing.c parsing/expanding.c \
			parsing/triming.c parsing/finalparse.c parsing/free.c \
			parsing/utils2.c
EXEC_F	=	minishell.c builtins/echo.c builtins/cd.c \
 			builtins/pwd.c builtins/export.c builtins/unset.c \
 			builtins/env.c builtins/exit.c execution/execution.c \
			execution/pipe.c execution/commands.c \
			execution/execute.c execution/files.c execution/ios.c \
			execution/utils.c execution/init.c execution/heredoc.c \
			execution/debug.c execution/free.c
FILES	=	$(PARS_F) $(EXEC_F)
OBJS	=	$(FILES:.c=.o)
LIBFT	=	libft/libft.a
FLAGS	=	-g -fsanitize=address #-Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(GREEN_BOLD)Linking $(NAME)...$(RESET)"
	@cc $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
	@echo "$(GREEN_BOLD)$(NAME) built successfully!$(RESET)"
	@echo "███╗   ███╗██╗███╗   ██╗██╗██╗  ██╗███████╗██╗     ██╗     "
	@echo "████╗ ████║██║████╗  ██║██║██║  ██║██╔════╝██║     ██║     "
	@echo "██╔████╔██║██║██╔██╗ ██║██║███████║█████╗  ██║     ██║     "
	@echo "██║╚██╔╝██║██║██║╚██╗██║██║██╔══██║██╔══╝  ██║     ██║     "
	@echo "██║ ╚═╝ ██║██║██║ ╚████║██║██║  ██║███████╗███████╗███████╗"
	@echo "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"

$(LIBFT):
	@echo "$(YELLOW_BOLD)Building libft...$(RESET)"
	@make -C libft

%.o: %.c
	@echo "$(BLUE_BOLD)Compiling $<...$(RESET)"
	@cc $(FLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW_BOLD)Cleaning object files...$(RESET)"
	@make -C libft clean
	@rm -f $(OBJS)

fclean: clean
	@echo "$(YELLOW_BOLD)Cleaning $(NAME)...$(RESET)"
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all

t:
	@$(MAKE) re
	@$(MAKE) clean
	@echo "$(READLINE_MSG)"

.PHONY: all clean fclean re t welcome
