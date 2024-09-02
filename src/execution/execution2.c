/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:30:29 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/09/02 17:41:04 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the given command exists in the linked list.
 *
 * @param token the head of the linked list
 * @param command the command to search for
 *
 * @return true if the command exists in the linked list, false otherwise
 *
 * The function walks through the linked list and checks if the command matches
 * one of the tokens in the list. If it does, it returns true. If it doesn't
 * find any match, it returns false.
 */
bool	builtin_filter(t_token *token, char *command)
{
	t_token	*tmp;

	if (command == NULL)
		return (false);
	tmp = token;
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(command, tmp->value, ft_strlen(command))
			&& tmp->type == BUILTIN)
			return (true);
		tmp = tmp->next;
	}
	tmp = NULL;
	return (false);
}

t_token	*find_token_exec(t_token *token, char **array)
{
	int			i;
	t_token		*tmp;

	i = 0;
	while (array[i])
	{
		tmp = token;
		while (tmp && tmp->next != NULL)
		{
			if (!ft_strncmp(array[i], tmp->value, ft_strlen(array[i]))
				&& tmp->type == BUILTIN)
			{
				tmp = search_backwards_for_builtin(tmp, array[i]);
				return (tmp);
			}
			tmp = tmp->next;
		}
		i++;
	}
	return (NULL);
}

/**
 * Handles the execution of built-in commands. It calls the built_ins() function
 * to execute the command and then exits the shell with the returned status.
 *
 * @param data the data structure holding the shell state
 * @param token the head of the linked list of tokens
 * @param env_ll the head of the environment linked list
 */
void	ft_builtin_exec(t_data *data, t_token *token, t_env **env_ll, int child)
{
	int	status;

	status = 0;
	if (token == NULL)
		exit(status);
	status = built_ins(data, token, env_ll, child);
	exit(status);
}

/**
 * Checks if the PATH environment variable is set in the environment linked list.
 *
 * @param env_ll the head of the environment linked list
 *
 * @return SUCCESS if the PATH variable is set, FAILURE otherwise
 */
int	check_path_unset(t_env **env_ll)
{
	t_env	*tmp;

	tmp = (*env_ll);
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(tmp->key, "PATH", 4))
			return (SUCCESS);
		tmp = tmp->next;
	}
	return (FAILURE);
}

/*
 * Handles the read end of the pipe after forking a new process.
 *
 * Closes the write end of the pipe and the previous read end of the pipe if
 * it is not the first command in the pipeline. Then assigns the read end of
 * the pipe to the read_end field of the data structure.
 * 
 * @param data the data structure holding the shell state
 */
void	handle_pipefd_readend(t_data *data)
{
	close(data->pipe_fd[1]);
	if (data->index > 0)
		close(data->read_end);
	data->read_end = data->pipe_fd[0];
}
