/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:17 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/29 15:56:01 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/**
 * Clean up all the memory used by the program and exit with the given
 * exit_code.
 *
 * This function is used to handle the cleanup of the program when we need to
 * exit with an error message. It first frees the array of strings passed to
 * it, then frees all the nodes in the environment linked list. Finally, it
 * calls free_data() to free the t_data structure, and then exits with the
 * given exit code.
 *
 * @param data The t_data structure to free.
 * @param env_ll The environment linked list to free.
 * @param cmd_array The array of strings to free.
 * @param exit_code The exit code to use when exiting.
 */
void	cleanup_and_exit(t_data *data, t_env **env_ll, char **cmd_array,
		int exit_code)
{
	free_array(cmd_array);
	free_all_ll(env_ll);
	free_data(data, NULL, NULL);
	exit(exit_code);
}


/**
 * The final execution function.
 * 
 * This function will check if the command is a built-in or not. If it is, it
 * will call the corresponding built-in function. If it is not, it will call
 * the execution_with_path() function to execute the command.
 * 
 * The function takes a t_data structure, a pointer to a linked list of
 * environment variables, and an array of strings as arguments.
 * 
 * If the command is not found, it will call the cleanup_and_exit() function
 * to free all the memory used and exit the program with an error message.
 * 
 * If the command is a built-in, the function will call the corresponding
 * built-in function and pass the arguments to it.
 * 
 * If the command is not a built-in, the function will call the
 * execution_with_path() function and pass the arguments to it.
 * 
 * The function does not return anything.
 */
void	ft_exec(t_data *data, t_env **env_ll, char **cmd_array)
{
	static char	*path;

	if (cmd_array[0] == NULL)
		exit (0);
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

int	tri_forking(t_data *data, t_env **env_ll, char ***all_cmds, pid_t pids)
{
	char sync_signal;

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
			tri_child_execution(data, env_ll, all_cmds[data->index], data->index);
		}
		else if (data->piped == true)
			handle_pipefd_readend(data);
		data->index++;
	}
	while (data->index--)
		wait(&data->status);
	return (data->status);
}

void	tri_child_execution(t_data *data, t_env **env_ll, char **cmd_with_args, int child)
{
	if (!cmd_with_args || !cmd_with_args[0])
	{
		free_all_ll(env_ll);
		free_data(data, NULL, NULL);
		exit(err_msg(NULL, "Invalid command", -1));
	}
	dup_fds(data, child, cmd_with_args);
	if (ft_strncmp(cmd_with_args[0], "cd", 3) == 0 && data->cd_executed)
	{
		free_all_ll(env_ll);
		free_data(data, NULL, NULL);
		exit(data->status);
	}
	if (data->redirections == true)
	{
		cmd_with_args = parse_instruction(data, cmd_with_args);
		if (!cmd_with_args || !*cmd_with_args)
		{
			free_all_ll(env_ll);
			free_data(data, NULL, cmd_with_args);
			exit(0);
		}
	}
	if (builtin_filter(data->token, cmd_with_args[0]) == true)
		ft_builtin_exec(data, find_token_exec(data->token, cmd_with_args), env_ll);
	else
		ft_exec(data, env_ll, cmd_with_args);
}
