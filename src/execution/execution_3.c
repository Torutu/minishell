/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:17 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/03 01:20:12 by walnaimi         ###   ########.fr       */
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

static void	update_path(t_env **env_ll, t_data *data)
{
	if (data->binary_paths)
		free_array(data->binary_paths);
	if (!*env_ll || !env_ll)
		return ;
	find_bin(env_ll, data);
	data->binary_paths = ft_split(data->bin, ':');
	if (!data->binary_paths)
		return ;
}

void	ft_exec(t_data *data, t_env **env_ll, char **cmd_array)
{
	static char	*path;

	update_path(env_ll, data);
	if (check_path_unset(env_ll))
		execution_absolute_path(data, cmd_array);
	data->env = env_arr_updater(env_ll);
	if (!data->env)
		exit(1);
	if (ft_strchr(cmd_array[0], '/') == NULL)
	{
		path = loop_path_for_binary(cmd_array[0], data->binary_paths);
		if (!path)
		{
			err_msg(cmd_array[0], NO_EXEC, 127);
			cleanup_and_exit(data, env_ll, cmd_array, 127);
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
	char	sync_signal;

	data->index = 0;
	signals(2);
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
			tri_child_exe(data, env_ll, all_cmds[data->index], data->index);
		}
		else if (data->piped == true)
			handle_pipefd_readend(data);
		data->index++;
	}
	return (data->status);
}

void	tri_child_exe(t_data *data, t_env **env_ll, char **cmd, int child)
{
	if (!cmd || !cmd[0])
	{
		free_all_resources(env_ll, data);
		exit(err_msg(NULL, "Invalid command", -1));
	}
	dup_fds(data, child, cmd);
	if (ft_strncmp(cmd[0], "cd", 3) == 0 && data->cd_executed)
	{
		free_all_resources(env_ll, data);
		exit(data->status);
	}
	if (data->redirections == true)
	{
		cmd = parse_instruction(data, cmd);
		if (!cmd || !*cmd)
		{
			free_all_with_cmd(env_ll, data, cmd);
			exit(0);
		}
	}
	if (builtin_filter(data->token, cmd[0]) == true)
		ft_builtin_exec(data, find_token_exec(data->token, cmd), env_ll, child);
	ft_exec(data, env_ll, cmd);
}
