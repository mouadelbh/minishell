NAME	=	minishell
PARS_F	=	minishell.c parsing/parsing.c parsing/lexer.c \
		parsing/tokenize.c parsing/utils.c parsing/checker.c \
		parsing/environment.c parsing/spacing.c parsing/expanding.c \
		parsing/triming.c parsing/finalparse.c parsing/free.c
# EXEC_F	=	minishell.c builtins/echo.c builtins/cd.c \
# 		builtins/pwd.c builtins/export.c builtins/unset.c \
# 		builtins/env.c builtins/exit.c execution/execution.c \
		execution/execution_utils.c
FILES	= $(PARS_F)
OBJS	=	$(FILES:.c=.o)
LIBFT	=	libft/libft.a
FLAGS	= #-fsanitize=address -g #-Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "Linking $(NAME)..."
	@cc $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
	@echo "$(NAME) built successfully!"

$(LIBFT):
	@echo "Building libft..."
	@make -C libft

%.o: %.c
	@echo "Compiling $<..."
	@cc $(FLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@make -C libft clean
	@rm -f $(OBJS)

fclean: clean
	@echo "Cleaning $(NAME)..."
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all

t:
	@$(MAKE) re
	@$(MAKE) clean

.PHONY: all clean fclean re t
