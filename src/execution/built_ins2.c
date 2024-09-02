/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/09/02 00:22:55 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Deletes the first node in the linked list of environment variables if its
 * content matches the value of the given token.
 *
 * @param env_ll A pointer to the 
 * head of the linked list of environment variables.
 * @param head The token whose value is to be compared with the content of the
 * first node.
 * @param data A structure containing the program's global data.
 *
 * @return 1 if the first node was deleted, 0 otherwise.
 */
int	delete_first_node(t_env **env_ll, t_token *head, t_data *data)
{
	t_env	*tmp;

	tmp = *env_ll;
	if (tmp && !ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
	{
		*env_ll = tmp->next;
		data->envll = *env_ll;
		free_null(tmp->key);
		free_null(tmp->value);
		free_null(tmp->content);
		free(tmp);
		return (1);
	}
	return (0);
}

/**
 * Deletes subsequent nodes in the linked list of environment variables if their
 * content matches the value of the given token.
 *
 * @param env_ll A pointer to the head 
 * of the linked list of environment variables.
 * @param head The token whose value is to be compared with the content of the
 * subsequent nodes.
 */
void	delete_subsequent_nodes(t_env *env_ll, t_token *head)
{
	t_env	*tmp;
	t_env	*del;

	tmp = env_ll;
	while (tmp != NULL && tmp->next != NULL)
	{
		if (!ft_strncmp(head->value, tmp->next->content,
				ft_strlen(head->value)))
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			free_null(del->key);
			free_null(del->value);
			free_null(del->content);
			free(del);
			break ;
		}
		tmp = tmp->next;
	}
}

/**
 * The unset built-in removes the given 
 * environment variables from the environment.
 *
 * The unset built-in takes multiple arguments, each of which is a name of an
 * environment variable. If the environment variable does not exist, the command
 * silently ignores it. If the environment
 *  variable exists, it is removed from the
 * environment.
 *
 * @param token The token containing the command and its arguments.
 * @param env_ll A pointer to the head of 
 * the linked list of environment variables.
 * @param data A structure containing the program's global data.
 *
 * @return 0 on success, -1 on failure.
 */
int	unset(t_token *token, t_env **env_ll, t_data *data)
{
	t_token	*head;

	head = token->next;
	while (head && head->value)
	{
		if (delete_first_node(env_ll, head, data))
		{
			head = head->next;
			continue ;
		}
		delete_subsequent_nodes(*env_ll, head);
		head = head->next;
	}
	return (SUCCESS);
}
