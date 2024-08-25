/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:27:10 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/25 18:44:09 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"
# include <stdbool.h>

typedef struct s_data	t_data;
typedef struct s_env	t_env;
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

typedef enum e_type
{
	BUILTIN = 1,
	COMMAND = 2,
	ARG = 3,
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
	bool			in_q;
	bool			empty;
	bool			echo;
	struct s_token	*next;
	struct s_token	*prev;
}		t_token;

// typedef struct s_hdoc_exp
// {
// 	t_env *envll;
// 	int		num_of_envs;
// 	int		env_len;
// 	int		s_quote_o;
// 	int		dbl_q;
	
	
// }

typedef struct s_vars
{
	int	i;
	int	j;
	int	single_q;
	int	double_q;
}	t_vars;

typedef struct s_add_spaces_vars
{
	char	*str;
	char	*new_str;
	int		*i;
	int		*j;
	char	ch;
}	t_add_spaces_vars;

typedef struct s_index
{
	int	i;
	int	j;
}	t_index;

char	*ft_strncpy(char *s1, const char *s2, int n);
/*****************************************
 * modify_str.c
 *****************************************/

//* ---------------------------------------------- */
//              src/parse/helper.c                 //
//* ---------------------------------------------- */
t_token	*find_token(t_token *token, t_type type);
int		search_token_type(t_token *token, t_type type);

//* ---------------------------------------------- */
//              src/parse/init_token.c             //
//* ---------------------------------------------- */
t_token	*init_token(void);

//* ---------------------------------------------- */
//              src/parse/tokenizer.c              //
//* ---------------------------------------------- */
int		line_tokenization(t_data *data);
t_token	*initialize_tokens(t_data *data);
t_token	*create_and_link_next_token(t_token *current_token, t_data *data);
int		chunky_checker(char *token, t_token *current_token, t_data *data);

//* ---------------------------------------------- */
//              src/parse/ft_strtok.c              //
/** ---------------------------------------------- */
char	*ft_strtok(char *str, t_data *data, t_token *cur_tok);
void	init_strtok(t_data *data);

//* ---------------------------------------------- */
//              src/parse/modify_str.c             //
//* ---------------------------------------------- */
char	*modify_str(char *str);
void	loop(char *str, char *n_str, int len, t_vars *pos);
void	process_chars(char *str, char *n_str, t_vars *pos);
void	add_spaces_d_c(char *str, char *n_str, int *i, int *j);
void	add_spaces_s_c(char *str, char *n_str, int *i, int *j);

//* ---------------------------------------------- */
//              src/parse/modify_str_utils.c       //
//* ---------------------------------------------- */
int		count_special_characters(const char *str);
char	*ft_strncpy(char *s1, const char *s2, int n);
int		ft_isspace(char c);
t_vars	init_vars(void);

//* ---------------------------------------------- */
//              src/parse/ft_strtok_utils.c        //
//* ---------------------------------------------- */
char	*skip_starting_delim(const char *str, const char *delim, char **target);
void	rm_quotes_and_skil_deli(const char *delim, t_data *data, char **target);
void	process_quotes_n_deli(const char *target, t_data *data);
int		unmatched_quote_check(t_data *data);
int		handle_tok(char *tok, t_token *c_t, t_data *d, char **tgt);

//* ---------------------------------------------- */
//              src/parse/ft_strtok_utils_1.c      //
//* ---------------------------------------------- */
int		ft_charinstr(char c, const char *str);
void	handle_quote(const char *target, t_data *data);
char	*substr_and_expand(const char *target, t_data *data);
void	quoted_segment(const char *str, char *n_str, t_data *data, char qt_c);
char	*remove_quotes(const char *str, t_data *data);

//*---------------------------------------------- */
//              src/parse/expand_env.c            //
//*---------------------------------------------- */
char	*expand_env_variables(const char *input, t_data *data);
void	dollar_sign(const char *str, t_index *num, t_data *data, char *result);
void	handle_status_variable(t_data *data, char *result, t_index *num);
char	*fetching_env(const char *str, t_index *num, t_data *data);
char	*get_env_value(const char *input, size_t v_s, size_t v_l, t_data *data);

//*----------------------------------------------*/
//              src/parse/expand_env_utils.c     //
//*----------------------------------------------*/
void	copy_env_value(char *result, const char *env_value, t_index *num);
void	setup_env_variables(const char *input, t_data *data);
void	single_q(const char *input, t_data *data, char *res, t_index *num);
void	double_q(const char *input, t_data *data, char *res, t_index *num);
void	handle_env_var(const char *str, t_index *num, t_data *data, char *res);

//*----------------------------------------------*/
//              src/parse/setup_env.c            //
//*----------------------------------------------*/
int		count_matching_keys(t_env *env_list, const char *input);
void	skip_non_word_characters(const char **ptr);
char	*extract_word(const char **ptr, int max_word_size);
int		is_key_in_list(t_env *head, const char *word);
int		ft_strccmp(char *s1, const char *s2);

//*----------------------------------------------*/
//              src/parse/ft_getenv.c            //
//*----------------------------------------------*/
char	*ft_getenv(char *key, t_env *env_ll);

//*---------------------------------------------- */
//              src/parse/chunky_check.c          //
//*---------------------------------------------- */
int		ft_argument_check(char *token, t_token *current_token);
int		ft_redirect_op_check(char *token, t_token *current_token);
int		ft_pipe_check(char *token, t_token *current_token);
int		cmd_check(char *token, t_token *current_token, t_data *data);
int		ft_builtin_check(char *token, t_token *current_token);

//*---------------------------------------------- */
//              src/parse/chunky_check_1.c        //
//*---------------------------------------------- */
int		check_builtin(char *token, t_token *current_token, t_data *data);
int		check_echo_flag(char *token, t_token *current_token, t_data *data);
int		check_flag(char *token, t_token *current_token, t_data *data);

//*---------------------------------------------- */
//              src/parse/chunky_check_2.c        //
//*---------------------------------------------- */
int		check_pipe(char *token, t_token *current_token, t_data *data);
int		check_redirect(char *token, t_token *current_token, t_data *data);
int		check_command(char *token, t_token *current_token, t_data *data);
int		check_argument(char *token, t_token *current_token, t_data *data);

//*---------------------------------------------- */
//              src/parse/chunky_utils.c          //
//*---------------------------------------------- */
char	*loop_path_for_binary(char *binary, char **paths);
char	*ft_strndup(const char *s, size_t n);
int		count_token(t_token *token, t_type type);
//*---------------------------------------------- */
//              src/parse/exe_check.c             //
//*---------------------------------------------- */
int		handle_cmd_exe(char *token, t_token *current_token, t_data *data);
void	handle_no_slash(char *exe_path, char *token, t_token *current_token);
void	handle_slash(char *exe_path, char *token, t_token *current_token);
int		handle_absolute_path(char *token, t_token *current_token);

//*---------------------------------------------- */
//              src/parse/redirection_check.c     //
//*---------------------------------------------- */
int		handle_redirect_out(char *token, t_token *current_token);
int		handle_append(char *token, t_token *current_token);
int		handle_redirect_in(char *token, t_token *current_token);
int		handle_heredoc(char *token, t_token *current_token);

#endif