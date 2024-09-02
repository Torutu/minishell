/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/09/02 21:45:28 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	struct stat	sb;
	int	fd;

	if (stat(array[0], &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		err_msg(array[0], " is a directory", 126);
		free_data(data, path, array);
		exit(126);
	}
	if (array[1] == NULL && data->redirections == false)
	{
		if (!isatty(STDIN_FILENO))
		{
			fd = open("/dev/tty", O_RDONLY);
			if (fd == -1) 
				exit(1);
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				close(fd);
				exit(1);
			}
			close(fd);
		}
	}
	if (execve(path, array, data->env) == -1)
	{
		err_msg(array[0], NO_EXEC, 127);
		free_data(data, path, array);
		exit(127);
	}
}

void	execution_absolute_path(t_data *data, char **array)
{
	struct stat	sb;
	int fd;

	if (stat(array[0], &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		err_msg(array[0], " is a directory", 126);
		free_data(data, NULL, array);
		exit(126);
	}
	if (array[1] == NULL && data->redirections == false)
	{
		if (!isatty(STDIN_FILENO))
		{
			fd = open("/dev/tty", O_RDONLY);
			if (fd == -1)
				exit(1);
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				close(fd);
				exit(1);
			}
			close(fd);
		}
	}
	if (execve(array[0], array, data->env) == -1)
	{
		err_msg(array[0], NO_EXEC, 127);
		free_data(data, NULL, array);
		exit(127);
	}
}
