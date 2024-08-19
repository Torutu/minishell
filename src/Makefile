CC = cc # CHANGE THIS

#Directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft
VPATH = src:libft:includes

# Readline flags
O_FLAGS = -lreadline -L${HOME}/.brew/opt/readline/lib

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -I${HOME}/.brew/opt/readline/include #-fsanitize=address #-g
INCFLAGS = -Iincludes -Ilibft/includes

# Main project files
SRC_FILES = main.c\
			init.c\
			utils.c\
			utils2.c\
			signals.c\
			ll_utils.c\
			ll_utils2.c\
			line_handler.c\
			src/execution/built_ins.c\
			src/execution/built_ins2.c\
			src/execution/execution.c\
			src/execution/redirections.c\
			src/execution/fd_dups.c\
			src/execution/redirect_parse.c\
			src/execution/execution_utils1.c\
			src/execution/execution_utils2.c\
			src/parse/tokenizer.c\
			src/parse/ft_strtok.c\
			src/parse/init_token.c\
			src/parse/token_test.c\
			src/parse/chunky_check.c\
			src/parse/chunky_check_1.c\
			src/parse/chunky_check_2.c\
			src/parse/check_utils.c\
			src/parse/freedom.c\
			src/parse/token_to_array.c\
			src/parse/getenv.c\
			src/parse/helper.c\
			src/parse/expand_env.c\
			src/parse/expand_env_utils.c\
			src/parse/modify_str.c\
			src/parse/ft_strtok_utils.c\
# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable
NAME = minishell

# Libft
LIBFT_MAKEFILE = $(LIBFT_DIR)/Makefile
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I$(LIBFT_DIR)/includes
LIBFT_LINK = -L$(LIBFT_DIR) -lft

all: $(NAME)
	@echo "\033[1;32m[✔] GOOD HEAVENS! LOOK AT THE EXECUTABLE!\033[0m"

%.o: %.c #this part has been changed
	@$(CC) $(CFLAGS) $(INCFLAGS) $(LIBFT_INC) -g -c $< -o $@

$(NAME): $(OBJ_FILES) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT_LINK) -o $(NAME) $(O_FLAGS)
	@echo "\033[1;33m[✔] Compiling minishell...\033[0m"

$(LIBFT): $(LIBFT_MAKEFILE)
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "\033[1;33m[✔] Linking to libft Makefile...\033[0m"

$(LIBFT_MAKEFILE):
	@echo "Creating symbolic link for libft Makefile..."
	@ln -s $(CURDIR)/$(LIBFT_MAKEFILE) $(LIBFT_MAKEFILE)

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "\033[1;33m[X] Cleaning...\033[0m"
	@rm -f $(OBJ_FILES)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "\033[1;31m[XXX] Cleaning it GOOOOOOD...\033[0m"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
