/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/19 10:57:48 by fdessoy-         ###   ########.fr       */
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
# define MALLOC "Malloc failure\n"
# define EXIT "Exit\n"
# define NO_EXEC "command not found"
# define NO_PERMISSION "permission denied"
# define REDIRECT_OUT 222
# define REDIRECT_IN 111
# define HERE_DOC 333
# define APP 444
# define NO_FILE 100
# define SYNTAX "syntax error near unexpected token "
# define NULL_LINE 5
# define DIRECTORY 69
# define FILE 55
# define EXECUTABLE 56
# define FILE_PERMISSION_DENIED 2
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
# define ERR_ARG "Wrong number of arguments, Karen\n"
# define SUCCESS 0
# define FAILURE 1

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
	char		**binary_paths;
	int			pipe_fd[2];
	int			fd_in;
	int			fd_out;
	char		*home_pwd;
	int			status;
	char		**cmd;
	t_token		*token;
	t_group		*groups;
	t_token		*first_node;
	t_token		*current_token;
	t_token		*prev_token;
	char		**cmd_a;
	bool		echoed;
	bool		echo_flag;
	bool		piped;
	bool		here_doc;
	bool		redirections;
	char		*line_read;
	int			id;
	char		*vtoken;
	const char *deli;
	bool		cmd_ignore;
	char		*ctoken;
	char		*cnew_token;
	int			quote;
	int			sindex;
	int			token_start;
	int			in_quotes;
	char		quote_char;
	size_t		len_t;
	int			s_quote_o;
	int			d_quote_o;
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
void	ft_exec(t_data *data, char **cmd_array, int child);

/* in redirections.c */
void	redirections_handling(t_data *data, char **array);
void	here_doc(t_data *data, char *delimiter);
int		find_redirection(char **array);

/* in execution_utils1.c */
int		err_msg(char *obj, char *msg, int err_code);
char	*access_path(char **path, char *cmd);
void	close_fds(t_data *data);

/* in execution_utils2.c */
char	**cl_to_array(t_token *token);
int		checking_access(t_data *data, char *instruction, int child);
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

/* in line_handler.c */
int		sniff_line(t_data *data);
int		syntax_check(t_token *token);
int		incorrect_syntax(t_token *token, t_type token_type);

/* in ll_utils.c */
t_env	*ft_listnew(void *content);
char	*get_home(t_env *env_ll);
void	ft_listadd_back(t_env **lst, t_env *new);
t_env	*ft_list_last(t_env *lst);
void	free_ll(t_env *env_ll);

/* in ll_utils2.c */
char	**env_arr_updater(t_env **env_ll);
int		ll_size(t_env **env_ll);

/* in built_ins.c */
int		built_ins(t_data *data, t_token *token, t_env **env_ll);
int		print_env(t_env *env_ll);
int		print_pwd(void);
void	get_the_hell_out(t_data *data, t_token *token, t_env *env_ll);
int		yodeling(t_token *token);

/* in built_ins2.c */
int		shell_cd(t_token *token, t_data *data);
int		export(t_token *token, t_env **env_ll);
int		print_export(t_env **env_ll);
int		unset(t_token *token, t_env **env_ll);
void 	alphabetical_printer(char **env_array);

/* signals.c */
void	handler(int sig);

/* DEPRECATED FUNCTIONS */
// int		built_in_or_garbage(t_data *data, t_env **env_ll, t_token *token);
// int		single_execution(t_data *data, t_token *token, t_env **env_ll); DEPRECATED
// void		single_child(t_data *data, t_token *token, t_env **env_ll); DEPRECRATED
// int		single_parent(pid_t pid, int status); DEPRECATED
// int 		lonely_execution(t_data *data, t_token *token, t_env **env_ll);
// int		how_many_children(t_token *token); DEPRECATED
// void		handle_heredoc(t_data *data, char *delimiter); DEPRECATED

#endif
