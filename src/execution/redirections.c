/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/14 10:06:07 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void redirections_handling(t_data *data, char **array)
{
	data->index = 0;
	while (array[data->index])
	{
		if (!ft_strcmp(array[data->index], "<"))
		{
			if (array[data->index + 1])
			{
				open_fdin(data, array[data->index + 1]);
				dup2(data->fd_in, STDIN_FILENO);
				close(data->fd_in);
				dup2(data->pipe_fd[1], STDOUT_FILENO);	
			}
			else
				exit(err_msg("'newline'", SYNTAX, 2));
		}
		else if (!ft_strcmp(array[data->index], ">"))
		{
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
		else if (!ft_strcmp(array[data->index], ">>"))
		{
			if (array[data->index + 1])
			{
				open_fdout(data, array[data->index + 1], 0);
				if (data->piped == true)
					dup2(data->read_end, STDIN_FILENO);
				dup2(data->fd_out, STDOUT_FILENO);
				close(data->fd_out);
			}
			else
				exit(err_msg("'newline'", SYNTAX, 2));		
		}
		else if (!ft_strcmp(array[data->index], "<<"))
		{
			if (array[data->index + 1])
			{
				here_doc(data, array[data->index + 1]);
				dup2(data->pipe_fd[0], STDIN_FILENO);
				close(data->pipe_fd[0]);
				close(data->pipe_fd[1]);
			}
			else
				exit(err_msg("'newline'", SYNTAX, 2));		
		}
		data->index++;
	}
}

void here_doc(t_data *data, char *delimiter)
{
	char	*input;

	while (1)
	{
		input = readline("8==D ");
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
			break ;
		write(data->pipe_fd[1], input, ft_strlen(input));
		write(data->pipe_fd[1], "\n", 1);
	}
}

int	find_redirection(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (!ft_strcmp(array[i], ">>")
			|| !ft_strcmp(array[i], "<<")
			|| !ft_strcmp(array[i], ">")
			|| !ft_strcmp(array[i], "<"))
			return (SUCCESS);
		i++;
	}
	return (FAILURE);
}