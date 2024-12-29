NAME	=	minishell
LIBF_FILES	= libft/ft_atoi.c libft/ft_bzero.c libft/ft_calloc.c libft/ft_isalnum.c libft/ft_isalpha.c libft/ft_isascii.c \
	libft/ft_isdigit.c libft/ft_isprint.c libft/ft_itoa.c libft/ft_memchr.c libft/ft_memcmp.c libft/ft_memcpy.c \
	libft/ft_memmove.c libft/ft_memset.c libft/ft_putchar_fd.c libft/ft_putendl_fd.c libft/ft_putnbr_fd.c \
	libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strchr.c libft/ft_strdup.c libft/ft_striteri.c libft/ft_strjoin.c \
	libft/ft_strlcat.c libft/ft_strlcpy.c libft/ft_strlen.c libft/ft_strmapi.c libft/ft_strncmp.c libft/ft_strnstr.c \
	libft/ft_strrchr.c libft/ft_strtrim.c libft/ft_substr.c libft/ft_tolower.c libft/ft_toupper.c libft/ft_lstadd_front.c \
	libft/ft_lstclear.c libft/ft_lstdelone.c libft/ft_lstiter.c libft/ft_lstmap.c libft/ft_lstnew.c libft/ft_lstsize.c \
	libft/ft_lstadd_back.c
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
HEADER_DIR = ./includes
HEADER	=	$(HEADER_DIR)/parsing.h $(HEADER_DIR)/minishell.h
OBJS	=	$(FILES:.c=.o)
LIBFT	=	libft/libft.a
FLAGS	=	-Wall -Wextra -Werror -g #-fsanitize=address

all: $(NAME)

$(NAME): $(OBJS) $(HEADER) $(LIBFT)
	@cc $(FLAGS) -I $(HEADER_DIR) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(LIBFT): $(LIBF_FILES) libft/libft.h
	@make -C libft

%.o: %.c $(HEADER)
	cc $(FLAGS) -c $< -o $@

clean:
	@make -C libft clean
	@rm -f $(OBJS)

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
