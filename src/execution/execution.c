/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 19:15:02 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// this function turns space into underscore,
// better used after echo and before cmd
void	replace_spaces_with_underscores(t_token *token_list)
{
	t_token	*current_token;
	int		i;

	current_token = token_list;
	while (current_token != NULL && current_token->value != NULL)
	{
		current_token->value_us = ft_strdup(current_token->value);
		if (!current_token->value_us)
			exit(err_msg(NULL, "Memory allocation failed", 1));

		i = 0;
		while (current_token->value_us[i] != '\0')
		{
			if (current_token->value_us[i] == ' ')
				current_token->value_us[i] = '_';
			i++;
		}
		current_token = current_token->next;
	}
}

/**
 * Prints the tokens in the linked list,
 * including their values, types, and other properties.
 * 
 * @param data A pointer to the t_data structure
 * containing the token linked list.
 * 
 * 
 * @return None
 */
// void    print_tokens(t_data *data)
// {
//     t_token    *token;
//     t_token    *last_token;

//     const char *type_names[]
//     = {
//         "UNKNOWN",
//         "BUILTIN",
//         "COMMAND",
//         "ARGUMENT",
//         "PIPE",
//         "FLAG",
//         "ENVVAR",
//         "RED_IN",
//         "RED_OUT",
//         "HEREDOC",
//         "APPEND",
//     };

//     token = data->token;
//     last_token = NULL;
//     while (token != NULL)
//     {
//         if (token->value != NULL)
//         {
//             printf("--------------[%d]--------------\n", token->id);
//             printf("token value    :[%s]\n", token->value);
// 			printf("token value_us :[%s]\n", token->value_us);
//             if (token->value[0] == '\0')
//                 printf("empty string\n");
//             printf("token type     :[%s]\n", type_names[token->type]);
//             if (token->empty == true)
//                 printf("empty?         :[%d]\n", token->empty);
//             if (token->in_q == true)
//                 printf("in quotes      :[%d]\n", token->in_q);
//             if (token->echo == true)
//                 printf("echo?          :[%d]\n", token->echo);
//             if (token->path != NULL)
//                 printf("token path     :[%s]\n", token->path);
//             printf("\n");
//             if (token->next == NULL)
//                 last_token = token;
//         }
//         token = token->next;
//     }
//     printf("#####################################\n");
//     token = last_token;
// }

/**
 * Execution and execution prepping are just the same function broke
 * into two parts for norminetting reasons. These functions mainly deal
 * with the general execution logic: separating built-ins execution from
 * all the other executions (piped, piped with redirections, just
 * redirection...). In here we fork and make the parent wait for execution
 * of the children. Another important part of it is the cl_to_array() that
 * turns our tokens into a 
 */
int	execution(t_data *data, t_env **env_ll)
{
	t_token	*token;

	token = data->token;
	if (token->value == NULL)
		return (0);
	data->nb_cmds = count_token(token, PIPE) + 1;
	if (data->nb_cmds == 0)
		data->nb_cmds = 1;
	if (!ft_strncmp(token->value, "cd", 3)
		|| !ft_strncmp(token->value, "export", 7)
		|| !ft_strncmp(token->value, "unset", 6)
		|| !ft_strncmp(token->value, "exit", 5))
		data->status = built_ins(data, token, env_ll);
	else
	{
		replace_spaces_with_underscores(token);
		data->status = execution_prepping(data, token, env_ll);
	}
	return (data->status);
}

int	execution_prepping(t_data *data, t_token *token, t_env **env_ll)
{
	static pid_t	pids;
	static char		**cmd_a;

	cmd_a = cl_to_array(token);
	if (!cmd_a)
		return (0);
	if (pipe(data->sync_pipe) == -1)
		return (err_msg(NULL, "Broken pipe\n", 141));
	data->status = forking(data, env_ll, cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	g_exit_code = 0;
	free_array(cmd_a);
	return (WEXITSTATUS(data->status));
}

int	forking(t_data *data, t_env **env_ll, char **all_cmds, pid_t pids)
{
	char	sync_signal;

	data->index = 0;
	g_exit_code = EXEC_SIG;
	while (data->index < data->nb_cmds)
	{
		if (data->piped == true && pipe(data->pipe_fd) == -1)
			return (err_msg(NULL, "Broken pipe\n", 141));
		pids = fork();
		if (pids < 0)
		{
			close_fds(data);
			return (err_msg(NULL, "Failed to fork\n", -1));
		}
		if (pids == 0)
		{
			if (data->index > 0 && data->heredoc_exist == true)
				read(data->sync_pipe[0], &sync_signal, 1);
			child_execution(data, env_ll, all_cmds[data->index], data->index);
		}
		else if (data->piped == true)
			handle_pipefd_readend(data);
		data->index++;
	}
	return (data->index);
}

/**
 * The piped execution is where the child processes go. Here we will check for
 * redirections to know if the user wants the output/input to be redirected
 * from/to a file.
 * 
 * RETURN VALUES: child_execution() does not return anything as it is just a
 * pathway to the final part of the execution in ft_exet().
 * 
 * DETAILS: at this point we may use exit() function without worrying that we
 * will end the whole program. Also, at this point we are working with fully
 * parsed out strings, our only concern should be if files/commands don't exist.
 * Examples of instruction:
 * "< infile cat"
 * "cat > outfile"
 * "ls -la Makefile"
 * "> outfile"
 * "<< END"
 */
void	child_execution(t_data *data, t_env **env_ll, char *instr, int child)
{
	char		**cmd_array;

	cmd_array = ft_split(instr, ' ');
	if (!cmd_array)
	{
		free_all_ll(env_ll);
		free_data(data, NULL, NULL);
		exit (err_msg(NULL, MALLOC, -1));
	}
	dup_fds(data, child, cmd_array);
	if (data->redirections == true)
	{
		cmd_array = parse_instruction(data, cmd_array);
		if (!cmd_array || !*cmd_array)
		{
			free_all_ll(env_ll);
			free_data(data, NULL, cmd_array);
			exit (0);
		}
	}
	if (builtin_filter(data->token, cmd_array[0]) == true)
		ft_builtin_exec(data, find_token_exec(data->token, cmd_array), env_ll);
	ft_exec(data, env_ll, cmd_array);
}

/**
 * This is the second part of the execution where we are going to
 * check if we have the redirection flag (int redirect) and we are
 * parsing the commands differently if we do.
 * 
 * Redirections here take an even more strict definition:
 * - "%> cat << EOF | cat > outfile"
 * 
 * Therefore, in here, redirections will be strictly "<" and ">", while
 * HERE_DOC and APPEND will have explicit naming because they are able
 * to take arguments beforehand.
 * 
 * [placeholder for more documentation]
 */
void	ft_exec(t_data *data, t_env **env_ll, char **cmd_array)
{
	static char	*path;

	if (cmd_array[0] == NULL)
		exit(0);
	if (check_path_unset(env_ll))
	{
		execution_absolute_path(data, cmd_array);
	}
	data->env = env_arr_updater(env_ll);
	if (!data->env)
		exit (1);
	if (ft_strchr(cmd_array[0], '/') == NULL)
	{
		path = loop_path_for_binary(cmd_array[0], data->binary_paths);
		if (!path)
		{
			err_msg(cmd_array[0], NO_EXEC, 127);
			free_array(cmd_array);
			free_all_ll(env_ll);
			free_data(data, NULL, NULL);
			exit(0);
		}
	}
	free_tokens(data->token);
	free_all_ll(env_ll);
	if (!path)
		execution_absolute_path(data, cmd_array);
	execution_with_path(data, cmd_array, path);
}
