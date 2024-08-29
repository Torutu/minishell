/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:28:13 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 04:04:41 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	input_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		if (!access(array[data->index + 1], F_OK))
		{
			open_fdin(data, array[data->index + 1]);
			dup2(data->fd_in, STDIN_FILENO);
			close(data->fd_in);
			if (data->piped == true)
				dup2(data->pipe_fd[1], STDOUT_FILENO);
		}
		else
			exit(err_msg(array[data->index + 1], FILE_ERROR, 2));
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void print_arry_with_args(char **cmd_with_args)
{
    if (cmd_with_args == NULL)
    {
        dprintf(2,"array is NULL\n");
        return;
    }
	dprintf(2, "array:\n");
    for (int i = 0; cmd_with_args[i] != NULL; i++)
    {
        dprintf(2, "%s\n", cmd_with_args[i]);
    }
}

void	output_redirection(t_data *data, char **array)
{
	print_arry_with_args(array);
	if (array[data->index + 1])
	{
		open_fdout(data, array[data->index + 1], 1);
		if (data->piped == true)
			dup2(data->read_end, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		close(data->fd_out);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	append_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		open_fdout(data, array[data->index + 1], 0);
		dprintf(2, "array: %s\n", array[data->index + 1]);
		if (data->piped == true)
			dup2(data->read_end, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		close(data->fd_out);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	heredoc_redirection(t_data *data, char **array)
{
	if (array[data->index + 1])
	{
		data->fd_in = here_doc(array[data->index + 1], data);
		dup2(data->fd_in, STDIN_FILENO);
		close(data->fd_in);
	}
	else
		exit(err_msg("'newline'", SYNTAX, 2));
}

void	check_and_handle_redirection(t_data *data, char **array)
{
	if (!ft_strncmp(array[data->index], "<", 2)
		&& ft_strlen(array[data->index]) == 1)
		input_redirection(data, array);
	else if (!ft_strncmp(array[data->index], ">", 2)
		&& ft_strlen(array[data->index]) == 1)
		output_redirection(data, array);
	else if (!ft_strncmp(array[data->index], ">>", 3)
		&& ft_strlen(array[data->index]) == 2)
		append_redirection(data, array);
	else if (!ft_strncmp(array[data->index], "<<", 3)
		&& ft_strlen(array[data->index]) == 2)
		heredoc_redirection(data, array);
}
