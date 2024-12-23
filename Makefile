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
			execution/debug.c execution/free.c builtins/export_utils.c \
			execution/checker.c execution/free_utils.c execution/command_node.c \
			builtins/export_utils_2.c
FILES	=	$(PARS_F) $(EXEC_F)
OBJS	=	$(FILES:.c=.o)
LIBFT	=	libft/libft.a
FLAGS	=	-g -fsanitize=address #-Wall -Wextra -Werror

all: $(NAME)

sanitize: $(S_NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(GREEN_BOLD)Linking $(NAME)...$(RESET)"
	@cc $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
	@echo "$(GREEN_BOLD)$(NAME) built successfully!$(RESET)"
	@echo "    ▄▄▄       ▄████▄   ▄▄▄█████▓ █    ██  ▄▄▄       ██▓        ██░ ██ ▓█████  ██▓     ██▓    "
	@echo "   ▒████▄    ▒██▀ ▀█   ▓  ██▒ ▓▒ ██  ▓██▒▒████▄    ▓██▒       ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    "
	@echo "   ▒██  ▀█▄  ▒▓█    ▄  ▒ ▓██░ ▒░▓██  ▒██░▒██  ▀█▄  ▒██░       ▒██▀▀██░▒███   ▒██░    ▒██░    "
	@echo "   ░██▄▄▄▄██ ▒▓▓▄ ▄██▒ ░ ▓██▓ ░ ▓▓█  ░██░░██▄▄▄▄██ ▒██░       ░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    "
	@echo "    ▓█   ▓██▒▒ ▓███▀ ░   ▒██▒ ░ ▒▒█████▓  ▓█   ▓██▒░██████▒   ░▓█▒░██▓░▒████▒░██████▒░██████▒"
	@echo "    ▒▒   ▓▒█░░ ░▒ ▒  ░   ▒ ░░   ░▒▓▒ ▒ ▒  ▒▒   ▓▒█░░ ▒░▓  ░    ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░"
	@echo "     ▒   ▒▒ ░  ░  ▒        ░    ░░▒░ ░ ░   ▒   ▒▒ ░░ ░ ▒  ░    ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░"
	@echo "     ░   ▒   ░           ░       ░░░ ░ ░   ░   ▒     ░ ░       ░  ░░ ░   ░     ░ ░     ░ ░   "
	@echo "         ░  ░░ ░                   ░           ░  ░    ░  ░    ░  ░  ░   ░  ░    ░  ░    ░  ░"
	@echo "            ░                                                                                "

$(S_NAME): $(S_OBJS) $(LIBFT)
	@echo "$(GREEN_BOLD)Linking $(S_NAME) with sanitizer...$(RESET)"
	@cc $(S_FLAGS) -o $(S_NAME) $(S_OBJS) $(LIBFT) -lreadline
	@echo "$(GREEN_BOLD)$(S_NAME) built successfully!$(RESET)"

$(LIBFT):
	@echo "$(YELLOW_BOLD)Building libft...$(RESET)"
	@make -C libft

%.o: %.c
	@echo "$(BLUE_BOLD)Compiling $<...$(RESET)"
	@cc $(FLAGS) -c $< -o $@

%.sanitize.o: %.c
	@echo "$(BLUE_BOLD)Compiling $< with sanitizer...$(RESET)"
	@cc $(S_FLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW_BOLD)Cleaning object files...$(RESET)"
	@make -C libft clean
	@rm -f $(OBJS) $(S_OBJS)

fclean: clean
	@echo "$(YELLOW_BOLD)Cleaning executables...$(RESET)"
	@make -C libft fclean
	@rm -f $(NAME) $(S_NAME)

re: fclean all

s_re: fclean sanitize

run: $(NAME)
	@./$(NAME)

s_run: $(S_NAME)
	@ASAN_OPTIONS=detect_leaks=0 ./$(S_NAME)

.PHONY: all clean fclean re sanitize s_re run s_run