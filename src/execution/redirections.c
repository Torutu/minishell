/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 13:04:39 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	last_heredoc(char **array)
{
	int	i;
	int	last_index;

	last_index = -1;
	i = 0;
	while (array[i])
	{
		if (!ft_strncmp(array[i], "<<", 2)
			&& ft_strlen(array[i]) == 2)
			last_index = i;
		i++;
	}
	return (last_index);
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

void	redirections_handling(t_data *data, char **array)
{
	int	last_heredoc_index;

	last_heredoc_index = last_heredoc(array);
	data->index = 0;
	while (array[data->index])
	{
		check_and_handle_redirection(data, array);
		if (!ft_strncmp(array[data->index], "<<", 2)
			&& ft_strlen(array[data->index]) == 2)
		{
			if (data->index == last_heredoc_index)
			{
				write(data->sync_pipe[1], "1", 1);
			}
		}
		data->index++;
	}
}

// void	redirections_handling(t_data *data, char **array)
// {
// 	int	last_heredoc_index;

// 	last_heredoc_index = last_heredoc(array);
// 	data->index = 0;
// 	while (array[data->index])
// 	{
// 		if (!ft_strncmp(array[data->index], "<", 1)
// 			&& ft_strlen(array[data->index]) == 1)
// 			input_redirection(data, array);
// 		else if (!ft_strncmp(array[data->index], ">", 1)
// 			&& ft_strlen(array[data->index]) == 1)
// 			output_redirection(data, array);
// 		else if (!ft_strncmp(array[data->index], ">>", 2)
// 			&& ft_strlen(array[data->index]) == 2)
// 			append_redirection(data, array);
// 		else if (!ft_strncmp(array[data->index], "<<", 2)
// 			&& ft_strlen(array[data->index]) == 2)
// 		{
// 			if (data->index == last_heredoc_index)
// 			{
// 				heredoc_redirection(data, array);
// 				write(data->sync_pipe[1], "1", 1);
// 			}
// 		}
// 		data->index++;
// 	}
// }

void	process_and_write_input(char *input, int *pipe_fd, t_data *data)
{
	char	*exp_input;

	exp_input = expand_env_variables(input, data);
	write(pipe_fd[1], exp_input, ft_strlen(exp_input));
	free_null(exp_input);
}

int	here_doc(char *delimiter, t_data *data)
{
	static char	*input;
	int			pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		exit(err_msg(NULL, "pipe error", 1));
	while (1)
	{
		g_exit_code = HEREDOC_SIG;
		input = readline(":3 ");
		if (!input)
		{
			close(pipe_fd[1]);
			return (pipe_fd[0]);
		}
		if (!ft_strncmp(input, delimiter, ft_strlen(delimiter)))
			break ;
		process_and_write_input(input, pipe_fd, data);
		write(pipe_fd[1], "\n", 1);
		free_null(input);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
