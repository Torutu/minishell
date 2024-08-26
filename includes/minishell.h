/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 02:05:41 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*************************************************/
/* User defined headers **************************/
/*************************************************/
# include "libft.h"
# include "../libft/includes/libft.h" // <- just to silence the nvim errors
# include "token.h"

/*************************************************/
/* external libraries ****************************/
/*************************************************/
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>

/*************************************************/
/* questionable libraries ************************/
/*************************************************/
# include <stdbool.h>
# include <limits.h>

/*************************************************/
/* macros ****************************************/
/*************************************************/
# define ERR "Error\n"
# define MALLOC "Malloc failure"
# define EXIT "Exit\n"
# define NO_EXEC ": command not found"
# define NO_PERMISSION "Permission denied"
# define HEREDOC_FAILURE "Unable to create temporary for here_doc"
# define HEREDOC_FAILURE2 "Unable to read temporary for here_doc"
# define FILE_ERROR " No such file or directory"
# define SYNTAX "syntax error near unexpected token "
# define ERR_ARG "Wrong number of arguments, Karen\n"
# define ERR_EXP "export: not a valid identifier\n"
# define EXEC_ENV_NULL "envir"
# define SYNTAX_EXIT ": exit: numeric argument required"
# define REDIRECT_OUT 222
# define REDIRECT_IN 111
# define HERE_DOC 333
# define APP 444
# define NO_FILE 100
# define NULL_LINE 5
# define DIRECTORY 69
# define FILE 55
# define EXECUTABLE 56
# define FILE_PERMISSION_DENIED 2
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
# define MAX_HEREDOC 1024
# define HEREDOC_SIG 1
# define EXEC_SIG 2
# define SUCCESS 0
# define FAILURE 1

/*************************************************/
/* global variable *******************************/
/*************************************************/
extern int	g_exit_code;

/*************************************************/
/* structs ***************************************/
/*************************************************/
typedef struct s_env
{
	char			*content;
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}			t_env;

typedef struct s_data
{
	char		**env;
	int			nb_cmds;
	int			read_end;
	int			*fd;
	char		*bin;
	int			index;
	char		*path;
	int			last_heredoc_index;
	char		**binary_paths;
	int			pipe_fd[2];
	int			sync_pipe[2];
	int			heredoc_fd[MAX_HEREDOC];
	int			fd_in;
	int			fd_out;
	char		*home_pwd;
	int			status;
	int			no_cmd_flag;
	char		**cmd;
	t_token		*token;
	t_token		*first_node;
	t_token		*cur_tok;
	t_token		*prev_token;
	t_index		indexx;
	char		**cmd_a;
	bool		echoed;
	bool		heredoc_exist;
	bool		echo_flag;
	bool		piped;
	bool		redirections;
	char		*line_read;
	char		*var_name;
	int			id;
	char		*tok_res;
	char		*tok_str;
	char		*new_tok;
	char		*fin_tok;
	const char	*deli;
	bool		ignore_cmd;
	char		*ctoken;
	char		*cnew_token;
	char		*str_token;
	int			quote;
	int			i;
	int			tok_srt;
	int			in_quotes;
	char		quote_char;
	size_t		env_len;
	int			num_of_envs;
	int			s_quote_o;
	int			dbl_q;
	int			exit_code;
	char		*new_str;
	char		*temp_str;
	t_env		*envll;
}	t_data;

/*************************************************/
/* functions *************************************/
/*************************************************/

/* in execution.c */
int		execution(t_data *data, t_env **env_ll);
int		execution_prepping(t_data *data, t_token *token, t_env **env_ll);
int		forking(t_data *data, t_env **env_ll, char **all_cmds, pid_t pids);
void	child_execution(t_data *data, t_env **env_ll, char *instr, int child);
void	ft_exec(t_data *data, t_env **env_ll, char **cmd_array);

/* in execution2.c */
bool	builtin_filter(t_token *token, char *command);
t_token	*find_token_exec(t_token *token, char **array);
void	ft_builtin_exec(t_data *data, t_token *token, t_env **env_ll);
int		check_path_unset(t_env **env_ll);
void	handle_pipefd_readend(t_data *data);

/* in syntax.c */
int		syntax_check(t_token *token);

/* in redirections.c */
int		find_redirection(char **array);
void	redirections_handling(t_data *data, char **array);
int		here_doc(char *delimiter, t_data *data);

/* in redirections_utils.c */
void	input_redirection(t_data *data, char **array);
void	output_redirection(t_data *data, char **array);
void	heredoc_redirection(t_data *data, char **array);
void	append_redirection(t_data *data, char **array);

/* in execution_utils1.c */
int		err_msg(char *obj, char *msg, int err_code);
char	*access_path(char **path, char *cmd);
void	close_fds(t_data *data);
void	execution_with_path(t_data *data, char **array, char *path);
void	execution_absolute_path(t_data *data, char **array);

/* in execution_utils2.c */
char	**cl_to_array(t_token *token);
int		fill_instr_loop(char **instruction, t_token **head);
int		alloc_memory(char ***pipe_array, char **instruction, \
					t_token **token);
t_token	*find_redtok(t_token *token);
char	*get_binary(char *instruction);

/* in fd_dups.c */
void	dup_fds(t_data *data, int child, char **array);
void	open_fdin(t_data *data, char *infile);
void	open_fdout(t_data *data, char *outfile, int flag);
void	exit_child(char *file, int err_code);

/* in redirect_parsing.c */
char	**parse_instruction(t_data *data, char **cmd_array);
char	**remove_redirect(t_data *data, char **array, int len);

/* in init.c */
void	ll_env(t_env **env_ll, char **env);
void	initializer(t_data *data, t_env **env_ll, char **env);
void	find_bin(t_env **env_ll, t_data *data);
char	*bin_extract(char *path);

/* in utils.c */
void	free_data(t_data *data, char *path, char **command_array);
void	free_token(t_token *token);
int		check_bin_local(char *binary);
int		check_bin_path(char *binary, char **paths);
int		is_file(char *binary, char *path);

/* in utils2.c */
void	malloc_check_message(void *ptr);
void	free_null(void *ptr);
void	super_free(t_data *data, t_env **env_ll);
int		wow_loop(t_data *data, t_env **env_ll);

/* in line_handler.c */
int		sniff_line(t_data *data);

/* in ll_utils.c */
t_env	*ft_listnew(void *content);
char	*get_home(t_env *env_ll);
void	ft_listadd_back(t_env **lst, t_env *new);
t_env	*ft_list_last(t_env *lst);
void	free_ll(t_env *env_ll);

/* in ll_utils2.c */
char	**env_arr_updater(t_env **env_ll);
int		ll_size(t_env **env_ll);
void	free_all_ll(t_env **env_ll);
char	**add_shell_lvl(char **env);
void	lstadd_front(t_env **lst, t_env *new);

/* in built_ins.c */
int		built_ins(t_data *data, t_token *token, t_env **env_ll);
int		print_env(t_env *env_ll);
int		print_pwd(void);
void	get_the_hell_out(t_data *data, t_token *token, t_env **env_ll);
int		yodeling(t_token *token);

/* in built_ins2.c */
int		shell_cd(t_token *token, t_data *data);
int		export(t_token *token, t_env **env_ll);
int		print_export(t_env **env_ll);
int		unset(t_token *token, t_env **env_ll);
void	alphabetical_printer(char **env_array);

/* signals.c */
void	handler(int sig);

/* in freeing.c */
void	*free_arr_retnull(char **array);
int		free_retstatus(char *array, int status);
void	free_tokens(t_token *head);
void	free_gang(t_data *data);
void	free_my_boi(char **paths);

/* DEPRECATED FUNCTIONS */
// int		built_in_or_garbage(t_data *data, t_env **env_ll, t_token *token);
// int		single_execution(t_data *data, t_token *token, t_env **env_ll);
// void		single_child(t_data *data, t_token *token, t_env **env_ll);
// int		single_parent(pid_t pid, int status);
// int 		lonely_execution(t_data *data, t_token *token, t_env **env_ll);
// int		how_many_children(t_token *token);
// void		handle_heredoc(t_data *data, char *delimiter);

#endif