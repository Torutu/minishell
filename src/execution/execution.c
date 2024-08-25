/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/25 18:03:22 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	data->nb_cmds = count_token(token, PIPE) + 1;
	if (data->nb_cmds == 0)
		data->nb_cmds = 1;
	if ((token->type == BUILTIN) && (!find_token(token, PIPE))
		&& (ft_strncmp(token->value, "cd", 2)
			|| ft_strncmp(token->value, "exit", 4)
			|| ft_strncmp(token->value, "export", 6)))
		data->status = built_ins(data, token, env_ll);
	else
		data->status = execution_prepping(data, token, env_ll);
	return (data->status);
}

int	execution_prepping(t_data *data, t_token *token, t_env **env_ll)
{
	static pid_t	pids;
	static char		**cmd_a;

	cmd_a = cl_to_array(token);
	if (!cmd_a)
		return (FAILURE);
	if (pipe(data->sync_pipe) == -1)
		return (err_msg(NULL, "Broken pipe\n", 141));
	data->status = forking(data, env_ll, cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	free_array(cmd_a);
	return (WEXITSTATUS(data->status));
}

int	forking(t_data *data, t_env **env_ll, char **all_cmds, pid_t pids)
{
	char	sync_signal;

	data->index = 0;
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
		exit(err_msg(cmd_array[0], NO_EXEC, 127));
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
