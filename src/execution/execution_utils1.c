/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 12:32:19 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*access_path(char **path, char *cmd)
{
	int		i;
	char	*curr_path;

	i = 0;
	while (path[i])
	{
		curr_path = ft_strsjoin(path[i], cmd, '/');
		if (!access(curr_path, F_OK))
		{
			if (!access(curr_path, X_OK))
			{
				free_array(path);
				return (curr_path);
			}
			ft_putstr_fd("Command not found: ", 2);
			ft_putendl_fd(cmd, 2);
		}
		free(curr_path);
		i++;
	}
	return (NULL);
}

/**
* This is our standard error printer.
* NEVER NULL THE FIRST PARAMETER!
*/
int	err_msg(char *obj, char *msg, int err_code)
{
	if (!obj && *msg)
		ft_putendl_fd(msg, 2);
	else if (*obj && *msg)
	{
		ft_putstr_fd(obj, 2);
		ft_putendl_fd(msg, 2);
	}
	return (err_code);
}

void	close_fds(t_data *data)
{
	if (data->pipe_fd[0] != 0)
		close(data->pipe_fd[0]);
	if (data->pipe_fd[1] != 0)
		close(data->pipe_fd[1]);
	if (data->fd_in != 0)
		close(data->fd_in);
	if (data->fd_out != 0)
		close(data->fd_out);
	if (data->read_end != 0)
		close(data->read_end);
	if (data->sync_pipe[1] != 0)
		close(data->sync_pipe[1]);
	if (data->sync_pipe[0] != 0)
		close(data->sync_pipe[0]);
}

void	execution_with_path(t_data *data, char **array, char *path)
{
	g_exit_code = EXEC_SIG;
	if (execve(path, array, data->env) == -1)
	{
		err_msg(array[0], NO_EXEC, 127);
		free_data(data, path, array);
		exit(127);
	}
}

void	execution_absolute_path(t_data *data, char **array)
{
	if (execve(array[0], array, data->env) == -1)
	{
		err_msg(array[0], NO_EXEC, 127);
		free_data(data, NULL, array);
		exit(127);
	}
}
