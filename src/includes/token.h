/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:35:39 by lstorey           #+#    #+#             */
/*   Updated: 2024/08/18 13:52:55 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define MAX_ARG_STR 10240
# include "minishell.h"
# include <stdbool.h>
typedef struct s_data t_data;
typedef struct s_env t_env;
typedef struct s_group t_group;
/*******************************************
 * enum assign types
 * 1. echo,cd,pwd...
 * 2. PATH=,ls, grep, chmod...
 * 3. , "Hello world"...
 * 4. |
 * 5. -n -l
 * 6. if it's an environment variable(probably gonna remove it)
 * 7. redirection input (<)
 * 8. redirection output (>)
 * 9. heredoc (<<)
 * 10. append (>>)
 * 404. not found (default).
 ********************************************/

typedef	enum e_type
{
	BUILTIN = 1,
	COMMAND = 2,
	ARGUMENT = 3,
	PIPE = 4,
	FLAG = 5,
	ENVVAR = 6,
	RED_IN = 7,
	RED_OUT = 8,
	HEREDOC = 9,
	APPEND = 10,
	UNKNOWN = 404,
}			t_type;

/*******************************************
 * type = takes from the enum above.
 * value = shows the exact value of the type.
 * id = index of the token.
 * expand = if token is gonna expands or not.
 * echo = if the token is part of token.
 * next = points to the next token.
 * prev = points to the previous token.
 *******************************************/

typedef struct s_token
{
	t_type			type;
	char			*value;
	char			*path;
	int				id;
	bool			expand;
	bool			echo;
	struct s_token	*next;
	struct s_token  *prev;
	struct s_group	*group;
}		t_token;

typedef struct s_group
{
	t_token *tokens;
	int		nb_tokens;
	int		id;
	struct s_group *next;
}		t_group;

/*****************************************
 * in src/parse/tokenizer.c
 *****************************************/
int		line_tokenization(t_data *data);
int		chunky_checker(char *token, t_token *current_token, t_data *data);
void 	free_tokens(t_token *head);

/*****************************************
 * src/parse/helper.c
 *****************************************/
t_token	*find_token(t_token *token, t_type type);
int		search_token_type(t_token *token, t_type type);



/*****************************************
 * src/parse/expand_env.c
 *****************************************/
char 	*expand_env_variable(const char *input, size_t *i, t_data *data, size_t *new_len);
char 	*expand_env_variables(const char *input, t_data *data);

/****************************************
 * src/parse/expand_env_utils.c
 ****************************************/
void 	setup_env_variables(const char *input,t_data *data);
void 	single_q(const char *input, t_data *data, char *res, size_t *i, size_t *j);
void 	double_q(const char *input, t_data *data, char *res, size_t *i, size_t *j);
void 	handle_env_variable(const char *input, size_t *i, t_data *data, char *result, size_t *j);
void 	copy_env_value(char *result, const char *env_value, size_t *j, t_data *data);

/*****************************************
 * in src/parse/ft_strtok.c
 *****************************************/
char	*ft_strtok(char *str, const char *delim, t_data *data, t_token *cur_tok);
int		ft_charinstr(char c, const char *str);
int		ft_strcmp(char *s1, char *s2);

/*****************************************
 * in src/parse/ft_strtok_utils.c
 *****************************************/
char 	*remove_quotes(const char *str, t_data *data);
char 	*skip_starting_delim(const char *str, const char *delim, char **target);

/*****************************************
 * in src/parse/echo.c
 *****************************************/
void 	echoing(t_token *current_token, t_token **prev_token, const char *delimiters, t_data *data);
char 	*concatenate_echo_args(t_token *current_token, const char *delimiters, t_data *data);
char 	*validate_and_process_token(const char *target, t_data *data);
void 	process_quoting_and_delimiters(const char *target, const char *delim, t_data *data, t_token *cur_tok);
void 	handle_quote(const char *target, t_data *data, t_token *cur_tok);

/*****************************************
 * in src/parse/init_token.c
 *****************************************/
t_token	*init_token();

/*****************************************
 * in src/parse/modify_str.c
 *****************************************/
void 	modify_str(char *str);

/*****************************************
 * in src/parse/chunky_check.c
 *****************************************/
int		ft_builtin_check(char *token, t_token *current_token);
int		ft_command_check(char *token, t_token *current_token, t_data *data);
int		ft_pipe_check(char *token, t_token *current_token);
int		ft_redirect_op_check(char *token, t_token *current_token);
int		ft_argument_check(char *token, t_token *current_token);

/***************************************
 * in src/parse/chunky_check_1.c
 ***************************************/
int		check_builtin(char *token, t_token *current_token, t_data *data);
int		check_echo_flag(char *token, t_token *current_token, t_data *data);
int		check_flag(char *token, t_token *current_token, t_data *data);

/***************************************
 * in src/parse/chunky_check_2.c
 ***************************************/
int		check_pipe(char *token, t_token *current_token, t_data *data);
int		check_redirect(char *token, t_token *current_token, t_data *data);
int		check_command(char *token, t_token *current_token, t_data *data);
int		check_argument(char *token, t_token *current_token, t_data *data);

/*****************************************
 * in src/parse/token_test.c
 *****************************************/
void	print_tokens(t_data *data);
void	print_cmd(char **cmd_a);

/*****************************************
 * in src/parse/check_utils.c
 *****************************************/
char	*loop_path_for_binary(char *binary, char **paths);
char	*ft_strndup(const char *s, size_t n);
int		how_many_tokens(t_token *token);
int		count_token(t_token *token, t_type type);
int		ft_argument_check(char *token, t_token *current_token);

/*****************************************
 * in src/parse/freedom.c
 * ***************************************/
void	free_my_boi(char **paths);

/******************************************
 * in src/parse/token_to_array.c
 * ****************************************/
char	**ttad(t_token *token_list, t_type delimiter);

/*******************************************
 * in src/parse/ft-getenv.c
 * *****************************************/
char	*ft_getenv(const char *token, t_env *env_ll);

# endif
