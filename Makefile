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
			freeing.c\
			freeing_1.c\
			ft_listnew.c\
			ll_utils.c\
			ll_utils2.c\
			line_handler.c\
			src/execution/cd.c\
			src/execution/built_ins.c\
			src/execution/built_ins2.c\
			src/execution/echoing.c\
			src/execution/execution.c\
			src/execution/execution2.c\
			src/execution/execution_3.c\
			src/execution/redirections.c\
			src/execution/redirections_utils.c\
			src/execution/fd_dups.c\
			src/execution/redirect_parse.c\
			src/execution/execution_utils1.c\
			src/execution/execution_utils2.c\
			src/execution/exporting.c\
			src/execution/exporting_utils.c\
			src/execution/exporting_utils_1.c\
			src/execution/exporting_utils_2.c\
			src/parse/tokenizer.c\
			src/parse/ft_strtok.c\
			src/parse/ft_strtok_utils.c\
			src/parse/ft_strtok_utils_1.c\
			src/parse/init_token.c\
			src/parse/syntax.c\
			src/parse/chunky_check.c\
			src/parse/chunky_check_1.c\
			src/parse/chunky_check_2.c\
			src/parse/redirection_check.c\
			src/parse/exe_check.c\
			src/parse/check_utils.c\
			src/parse/getenv.c\
			src/parse/setup_env.c\
			src/parse/expand_env.c\
			src/parse/expand_env_utils.c\
			src/parse/modify_str.c\
			src/parse/modify_str_utils.c\
			src/parse/helper.c\

GREEN = \033[1;32m
YELLOW = \033[1;33m
RED = \033[1;31m
RESET = \033[0m


# Object files
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Executable
NAME = minishell

# Libft
LIBFT_MAKEFILE = $(LIBFT_DIR)/Makefile
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I$(LIBFT_DIR)/includes
LIBFT_LINK = -L$(LIBFT_DIR) -lft

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCFLAGS) $(LIBFT_INC) -g -c $< -o $@

$(NAME): $(OBJ_FILES) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT_LINK) -o $(NAME) $(O_FLAGS)
	@echo -e "$(GREEN)[✔] Compiling minishell...\033[0m"
	@echo -e "$(GREEN) ___  ____       _     _          _ _ \033[0m"
	@echo -e "$(GREEN) |  \/  (_)     (_)   | |        | | | \033[0m"
	@echo -e "$(GREEN) | .  . |_ _ __  _ ___| |__   ___| | | \033[0m"
	@echo -e "$(GREEN) | |\/| | | '_ \| / __| '_ \ / _ \ | |\033[0m"
	@echo -e "$(GREEN) | |  | | | | | | \__ \ | | |  __/ | | \033[0m"
	@echo -e "$(GREEN) \_|  |_/_|_| |_|_|___/_| |_|\___|_|_| [WB] \033[0m"

$(LIBFT): $(LIBFT_MAKEFILE)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@echo -e "$(GREEN)[✔] Linking to libft Makefile...\033[0m"

$(LIBFT_MAKEFILE):
	@echo -e "Creating symbolic link for libft Makefile..."
	@ln -s $(CURDIR)/$(LIBFT_MAKEFILE) $(LIBFT_MAKEFILE)

clean:
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
	@echo -e "$(YELLOW)removing obj files...\033[0m"
	@rm -rdf $(OBJ_DIR)

fclean: clean
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
	@echo -e "$(RED)removing executable...\033[0m"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
