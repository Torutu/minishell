/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:03:21 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/09/01 22:59:14 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	last_heredoc(t_token *token)
{
	int	i;
	int	last_index;

	last_index = -1;
	i = 0;
	while (token->value)
	{
		if (token->type == HDOC)
			last_index = i;
		token = token->next;
		i++;
	}
	return (last_index);
}

// int	find_redirection(char **array)
// {
// 	int	i;

// 	i = 0;
// 	while (array[i])
// 	{
// 		if (!ft_strcmp(array[i], ">>")
// 			|| !ft_strcmp(array[i], "<<")
// 			|| !ft_strcmp(array[i], ">")
// 			|| !ft_strcmp(array[i], "<"))
// 			return (SUCCESS);
// 		i++;
// 	}
// 	return (FAILURE);
// }

int	find_redirection(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp->value)
	{
		if (search_token_type(tmp, RED_IN)
			|| search_token_type(tmp, RED_OUT)
			|| search_token_type(tmp, APPEND)
			|| search_token_type(tmp, HDOC))
			return (SUCCESS);
		tmp = tmp->next;
	}
	tmp = NULL;
	return (FAILURE);
}

void	redirections_handling(t_data *data, char **array)
{
	int		last_heredoc_index;
	t_token	*token;

	token = data->token;
	last_heredoc_index = last_heredoc(data->token);
	data->index = 0;
	while (array[data->index] && token->value)
	{
		check_and_handle_redirection(data, array);
		if (!ft_strncmp(array[data->index], "<<", 2) && token->type == HDOC)
		{
			if (data->index == last_heredoc_index)
				write(data->sync_pipe[1], "1", 1);
		}
		token = token->next;
		data->index++;
	}
}

void	process_and_write_input(char *input, int *pipe_fd, t_data *data)
{
	char	*exp_input;

	exp_input = expand_env_variables(input, data);
	write(pipe_fd[1], exp_input, ft_strlen(exp_input));
	free_dock((void **)&exp_input);
}

int	here_doc(char *delimiter, t_data *data)
{
	static char	*input;
	int			pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		exit(err_msg(NULL, "pipe error", 1));
	while (1)
	{
		signals(3);
		input = readline(":3 ");
		if (!input)
		{
			close(pipe_fd[1]);
			return (pipe_fd[0]);
		}
		if (!ft_strncmp(input, delimiter, ft_strlen(input) + 1))
			break ;
		process_and_write_input(input, pipe_fd, data);
		write(pipe_fd[1], "\n", 1);
		free_null(input);
	}
	free_null(input);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
