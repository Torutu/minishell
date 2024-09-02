/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:30:29 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/09/02 03:06:15 by bposa            ###   ########.fr       */
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

/**
 * Given an array of strings, go through the linked list and return the
 * address of the first node that matches one of the strings in the array.
 * The comparison is done with strncmp and the type of the node has to be
 * BUILTIN. If there is more than one match, the function will return the
 * last matching node.
 *
 * @param token the head of the linked list
 * @param array the array of strings to compare with
 *
 * @return the address of the first matching node or NULL if no match is found
 */
t_token	*find_token_exec(t_token *token, char **array)
{//instead of checkin for BUILTIN in end token pointer, maybe check for echo?
	int		i;
	t_token	*tmp;
	t_token	*end;

	i = 0;
	tmp = token;
	end = token;
	while (end && end->next && end->next->next != NULL)
		end = end->next;
	while (array[i])
	{
		tmp = token;
		while (tmp && tmp->next != NULL)
		{
			if (!ft_strncmp(array[i], tmp->value, ft_strlen(array[i]))
				&& tmp->type == BUILTIN)
			{
				while (end != tmp && !(end->type == BUILTIN && !ft_strncmp(array[i], end->value, ft_strlen(array[i]))))
					end = end->prev;
				if (end != tmp)
					tmp = end;
				return (tmp);
			}
			tmp = tmp->next;
		}
		i++;
	}
	tmp = NULL;
	end = NULL;
	return (NULL);
}

void	ft_builtin_exec(t_data *data, t_token *token, t_env **env_ll)
{
	int	status;

	status = 0;
	if (token == NULL)
		exit(status);
	status = built_ins(data, token, env_ll);
	exit(status);
}

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

void	handle_pipefd_readend(t_data *data)
{
	close(data->pipe_fd[1]);
	if (data->index > 0)
		close(data->read_end);
	data->read_end = data->pipe_fd[0];
}
