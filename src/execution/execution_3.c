/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:17 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/28 12:56:16 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	cleanup_and_exit(t_data *data, t_env **env_ll, char **cmd_array,
		int exit_code)
{
	free_array(cmd_array);
	free_all_ll(env_ll);
	free_data(data, NULL, NULL);
	exit(exit_code);
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
		execution_absolute_path(data, cmd_array);
	data->env = env_arr_updater(env_ll);
	if (!data->env)
		exit (1);
	if (ft_strchr(cmd_array[0], '/') == NULL)
	{
		path = loop_path_for_binary(cmd_array[0], data->binary_paths);
		if (!path)
		{
			err_msg(cmd_array[0], NO_EXEC, 127);
			cleanup_and_exit(data, env_ll, cmd_array, 0);
		}
	}
	free_tokens(data->token);
	free_all_ll(env_ll);
	if (!path)
		execution_absolute_path(data, cmd_array);
	execution_with_path(data, cmd_array, path);
}
